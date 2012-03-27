using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Configuration;
using System.Threading;

using Emgu.CV;
using Emgu.Util;
using Emgu.CV.Structure;
using ImageSegmentationDemo.Helper;
using ImageSegmentationDemo.Strategy;
using System.IO;


namespace ImageSegmentationDemo
{
    public partial class Form1 : Form
    {
        private Point _leftTop = new Point();
        private Point _rightBottom = new Point();
        private Graphics _painter = null;
        private Image<Bgr, Byte> _totalImage = null;
        private Image<Bgr, Byte> _originalImage = null;
        private Bitmap _afterPre = null;
        private List<Bitmap> _resultImages = new List<Bitmap>();
        private Rectangle _selectedRect;
        private Rectangle _selectedSurfRect;
        private bool _done = false;
        private ImageAssets _imageAssets = new ImageAssets(ConfigurationManager.AppSettings["inputPath"]);
        private ISegmentation _segmentation;
        private double _scale = 0.2;
        private StreamWriter sw = new StreamWriter("log.txt");
        public Form1()
        {
            InitializeComponent();            
            this.pictureBoxCT.ImageLocation = _imageAssets.FileList[0];
            this.pictureBoxCT.Load();
            _originalImage = new Image<Bgr, Byte>((Bitmap)this.pictureBoxCT.Image);
        }

        //catch the event mouse down, start to do the interaction with users.
        private void pictureBoxCT_MouseDown(object sender, MouseEventArgs e)
        {
            if (_painter == null)
            {
                _painter = this.pictureBoxCT.CreateGraphics(); 
            }
            if (!_leftTop.IsEmpty)
            {
                this.pictureBoxCT.Refresh();
            }
            _leftTop = e.Location;
            _done = false;
        }

        //catch the event mouse move, and paint the rectangle in process
        private void pictureBoxCT_MouseMove(object sender, MouseEventArgs e)
        {
            if (_painter != null && _done == false)
            {
                _rightBottom = e.Location;
                Pen bluePen = new Pen(Color.Blue, 3);
                _selectedRect = new Rectangle(_leftTop, new Size(_rightBottom.X - _leftTop.X, _rightBottom.Y - _leftTop.Y));
                this.pictureBoxCT.Refresh();
                _painter.DrawRectangle(bluePen, _selectedRect);
            }
        }

        //catch the event mouseup, and set the regtangle's c0ordinate
        private void pictureBoxCT_MouseUp(object sender, MouseEventArgs e)
        {
            _rightBottom = e.Location;
            this.setSelectedRectangle();
            Pen redPen = new Pen(Color.Red, 3);
            _painter.DrawRectangle(redPen, _selectedSurfRect);
            _done = true;
        }

        //find bones mainly, for single picture, find the tumor.
        private void buttonFind_Click(object sender, EventArgs e)
        {
            this.segmentImages();
            //Thread segmentThread = new Thread(new ThreadStart(this.segmentImages));
            //segmentThread.Start();
            //Thread.Sleep(1);
            //this.pictureBoxCT.Image = _segmentation.Segment((Bitmap)this.pictureBoxCT.Image, _originalImage, _selectedRect);
            //this.pictureBoxCT.Refresh();
            //Pen redPen = new Pen(Color.Red, 3);
            //_painter.DrawRectangle(redPen, _selectedSurfRect);
        }

        //segment images using grabcut, mainly segment the bones
        private void segmentImages()
        {
            _segmentation = new GrabCutSegmentation();
            Pen redPen = new Pen(Color.Red, 3);
            int index = 1;
            string imageLocation;
            while ((imageLocation = this._imageAssets.GetFirstAsset()) != string.Empty)
            {
                Image<Bgr, Byte> CTImage = new Image<Bgr, byte>(imageLocation);
                Bitmap bitmapImage = CTImage.ToBitmap();
                Bitmap result = _segmentation.Segment(bitmapImage, CTImage, _selectedRect);
                if (checkBitmap(result))
                {
                    _resultImages.Add(result);
                    this.pictureBoxCT.Image = result;
                    this.pictureBoxCT.Refresh();
                    System.Console.WriteLine(index++);
                    this._imageAssets.RemoveFirstAsset();
                    result.Save(imageLocation.Replace(".bmp", "new.bmp"));
                }
                else
                {
                    this.pictureBoxCT.ImageLocation = imageLocation;
                    this.pictureBoxCT.Load();
                    break;
                }
            }
            //index = 1;
            //foreach (Bitmap resultImage in resultImages)
            //{
            //    this.pictureBoxCT.Image = resultImage;
            //    this.pictureBoxCT.Refresh();
            //    _painter.DrawRectangle(redPen, _selectedSurfRect);
            //    MessageBox.Show("This the " + index + "result");
            //    index++;
            //}
            GC.Collect();
        }

        //open a picture
        private void buttonOpen_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.CheckFileExists = true;
            ofd.DefaultExt = "bmp";
            ofd.Filter = "图片文件|*.jpg;*.png;*.bmp|所有文件|*.*";
            if (ofd.ShowDialog(this) == DialogResult.OK)
            {
                if (ofd.FileName != "")
                {
                    this.pictureBoxCT.ImageLocation = ofd.FileName;
                    this.pictureBoxCT.Load();
                    _originalImage = new Image<Bgr, Byte>((Bitmap)this.pictureBoxCT.Image);
                }
            }
            ofd.Dispose();
        }

        //save the pictures showed in the picturebox
        private void buttonSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.CheckPathExists = true;
            sfd.DefaultExt = "bmp";
            sfd.Filter = "图片文件|*.bmp";
            if (sfd.ShowDialog(this) == DialogResult.OK)
            {
                if (sfd.FileName != "")
                {
                    System.IO.FileStream fs = (System.IO.FileStream)sfd.OpenFile();
                    this.pictureBoxCT.Image.Save(fs, System.Drawing.Imaging.ImageFormat.Bmp);
                    fs.Close();
                }
            }
            sfd.Dispose();
           
        }

        //help information
        private void buttonHelp_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Please use your mouse to draw a rectangle bounding the tumor, then press Button \"Find Boundary\".");
        }

        //Preprocess, mainly stretching the grey level distribution
        private void buttonPreprocess_Click(object sender, EventArgs e)
        {
           
            int index = 0;
            foreach (Bitmap result_bone in this._resultImages)
            {
                Dictionary<Color, int> hist = new Dictionary<Color, int>();
                for (int i = 0; i < result_bone.Height; i++)
                {
                    for (int j = 0; j < result_bone.Width; j++)
                    {
                        Color color = result_bone.GetPixel(i, j);
                        if (color.Name != "ff000000")
                        {
                            if (hist.ContainsKey(color))
                            {
                                hist[color]++;
                            }
                            else
                            {
                                hist.Add(color, 1);
                            }
                        }
                    }
                }
                Color max = Color.Black;
                Color min = Color.White;
                foreach (Color color in hist.Keys)
                {
                    if (color.R > 100)
                    {
                        if (color.R > max.R)
                            max = color;
                        if (color.R < min.R)
                            min = color;
                    }
                }
                Dictionary<Color, Color> colorConvert = new Dictionary<Color, Color>();
                for(int i=0; i<result_bone.Height; i++)
                    for (int j = 0; j < result_bone.Width; j++)
                    {
                        Color color = result_bone.GetPixel(i, j);
                        if (colorConvert.ContainsKey(color))
                            result_bone.SetPixel(i, j, colorConvert[color]);
                        else
                        {
                            if (color.R > 100)
                            {
                                int A = 255;
                                int R = (int)System.Math.Floor((color.R - min.R) * (255.0 - 0.0) / (max.R - min.R));
                                int G = R;
                                int B = R;
                                Color newColor = Color.FromArgb(A, R, G, B);
                                colorConvert.Add(color, newColor);
                                result_bone.SetPixel(i, j, colorConvert[color]);
                            }
                        }
                    }

                result_bone.Save(ConfigurationManager.AppSettings["outputPath"] + "result_bone" + index.ToString() + ".bmp");
                this._afterPre = new Bitmap(result_bone);
                index++;
                this.pictureBoxCT.Image = result_bone;
                this.pictureBoxCT.Refresh();
            }
        }


        private void pictureBoxCT_LoadCompleted(object sender, AsyncCompletedEventArgs e)
        {
            _originalImage = new Image<Bgr, Byte>((Bitmap)this.pictureBoxCT.Image);
        }

        //find tumor button is clicked
        private void buttonFindTumor_Click(object sender, EventArgs e)
        {
            int index = 0;
            foreach (Bitmap result_tumor in this._resultImages)
            {
                Dictionary<List<int>, int> diff_result = new Dictionary<List<int>, int>();
                Dictionary<List<int>, double> similar_result = new Dictionary<List<int>, double>();
                for (int i = 0; i < result_tumor.Height; i++)
                {
                    for (int j = 0; j < result_tumor.Width; j++)
                    {
                        Color color = result_tumor.GetPixel(i, j);
                        if (color.Name != "ff000000")
                        {
                            //detect the tumor by the vectors' distance;
                            int diff = getDiff(result_tumor, i, j, 5);
                            diff_result.Add(new List<int> { i, j }, diff * color.R);
                            
                            //detect the tumor by the cosin
                            //double similar = getSimilar(result_tumor, i, j, 3);

                            //detect the tumor by the cov
                            double similar = getCov(result_tumor, i, j, 3);
                            similar_result.Add(new List<int> { i, j }, similar);
                        }
                    }
                }

                foreach (List<int> coordinate in similar_result.Keys)
                {
                    if (similar_result[coordinate] > 160)
                    {
                        result_tumor.SetPixel(coordinate[0], coordinate[1], Color.Red);
                    }
                }

                foreach (List<int> coordinate in diff_result.Keys)
                {
                    if (diff_result[coordinate] < 400)
                    {
                        result_tumor.SetPixel(coordinate[0], coordinate[1], Color.Blue);
                    }
                }
                sw.WriteLine("diff:");
                foreach(List<int> key in diff_result.Keys)
                    sw.WriteLine(diff_result[key]);
                sw.WriteLine("similar:");
                foreach (List<int> key in similar_result.Keys)
                {
                    if(Double.IsNaN(similar_result[key]))
                        sw.WriteLine(0);
                    else
                        sw.WriteLine(similar_result[key]);
                }
                sw.Flush();
                //afterProcess(result_tumor, 5, 0.4).Save(ConfigurationManager.AppSettings["outputPath"] + "afterprocess_cov_and_distance_5tumor" + index.ToString() + "400"+ ".bmp");
                afterProcess(result_tumor, 8).Save(ConfigurationManager.AppSettings["outputPath"] + "afterprocess_v2_cov_and_distance_5tumor" + index.ToString() + "400" + ".bmp");
                //result_tumor.Save(ConfigurationManager.AppSettings["outputPath"] + "afterprocess_cov_and_distance_5tumor" + index.ToString() + "400"+ ".bmp");
                index++;
            }
        }

        #region helper function
        //get the selected rectangle of the 512*512 window and adjust it to the image's original size.
        private void setSelectedRectangle()
        {
            int height = _originalImage.Height;
            int width = _originalImage.Width;
            _selectedSurfRect = new Rectangle(_leftTop, new Size(_rightBottom.X - _leftTop.X, _rightBottom.Y - _leftTop.Y));
            _leftTop = new Point(_leftTop.X * width / 512, _leftTop.Y * height / 512);
            _rightBottom = new Point(_rightBottom.X * width / 512, _rightBottom.Y * height / 512);
            _selectedRect = new Rectangle(_leftTop, new Size(_rightBottom.X - _leftTop.X, _rightBottom.Y - _leftTop.Y));
        }

        //check the result bitmap, if the segmentation result is too small, then returns false.
        private bool checkBitmap(Bitmap resultImage)
        { 
            int noneZeroPixelNum = 0;
            for (int i = 0; i < resultImage.Height; i++ )
            {
                for (int j = 0; j < resultImage.Width; j++ )
                {
                    Color color = resultImage.GetPixel(i, j);
                    if (color.Name != "ff000000")
                        noneZeroPixelNum++;
                }
            }
            System.Console.WriteLine(noneZeroPixelNum);
            double threshold =_selectedRect.Height * _selectedRect.Width * _scale;
            System.Console.WriteLine("Threshold:" + threshold);
            if (noneZeroPixelNum < threshold)
            {
                _scale = _scale > 0.01 ? _scale - 0.01 : _scale;
                return false;
            }
            _scale = _scale < 1 ? _scale + 0.002 : _scale;
            return true;
        }

        //use the distance of two vectors to determine whether a pixel belongs to a tumor or not.
        private int getDiff(Bitmap result, int x, int y, int width)
        {
            int diff = 0;
            int start_x = x - (width - 1) / 2;
            int end_x = x + (width - 1) / 2;
            int start_y = y - (width - 1) / 2;
            int end_y = y + (width - 1) / 2;
            for (int i = start_x; i < end_x; i++)
            {
                for (int j = start_y; j < end_y; j++)
                {
                    diff += Math.Abs(result.GetPixel(i, j).R - result.GetPixel(x, y).R);
                }
            }
            return diff;
        }

        //use the similarity coefficients of two vectors to determine whether a pixel belongs to a tumor or not.
        private double getSimilar(Bitmap result, int x, int y, int width)
        {
            double similar = 0;
            double similar_top = 0;
            double similar_right = 0;
            double similar_bottom = 0;
            double similar_left = 0;
            double length = 0;
            double length_top = 0;
            double length_right = 0;
            double length_bottom = 0;
            double length_left = 0;
            int start_x = x - (width - 1) / 2;
            int end_x = x + (width - 1) / 2;
            int start_y = y - (width - 1) / 2;
            int end_y = y + (width - 1) / 2;
            for (int i = start_x; i < end_x; i++)
            {
                for (int j = start_y; j < end_y; j++)
                {
                    similar_top += (Math.Abs((result.GetPixel(i, j).R + 1) * (result.GetPixel(i - width, j).R + 1)));
                    similar_right += (Math.Abs((result.GetPixel(i, j).R + 1) * (result.GetPixel(i, j + width).R + 1)));
                    similar_bottom += (Math.Abs((result.GetPixel(i, j).R + 1) * (result.GetPixel(i + width, j).R + 1)));
                    similar_left += (Math.Abs((result.GetPixel(i, j).R + 1) * (result.GetPixel(i, j - width).R + 1)));

                    length += (result.GetPixel(i, j).R+1) * (result.GetPixel(i, j).R+1);
                    length_top += (result.GetPixel(i - width, j).R+1) * (result.GetPixel(i - width, j).R+1);
                    length_right += (result.GetPixel(i, j + width).R+1) * (result.GetPixel(i, j + width).R+1);
                    length_bottom += (result.GetPixel(i + width, j).R+1) * (result.GetPixel(i + width, j).R+1);
                    length_left += (result.GetPixel(i, j - width).R+1) * (result.GetPixel(i, j - width).R+1);
                }
            }
            length = Math.Sqrt(length);
            length_top = Math.Sqrt(length_top);
            length_right = Math.Sqrt(length_right);
            length_bottom = Math.Sqrt(length_bottom);
            length_left = Math.Sqrt(length_left);

            similar_top = (length * length_top == 0) ? 0 : similar_top / (length * length_top);
            similar_right = (length * length_right == 0) ? 0 : similar_right / (length * length_right);
            similar_bottom = (length * length_bottom == 0) ? 0 : similar_bottom / (length * length_bottom);
            similar_left = (length * length_left == 0) ? 0 : similar_left / (length * length_left);

            similar = similar_top + similar_right + similar_bottom + similar_left;

            similar = similar * (255 - result.GetPixel(x, y).R);
            return similar;
        }

        //use the related coefficients of two vectors to determin whether a pixel belongs to a tumor or not.
        private double getCov(Bitmap result, int x, int y, int width)
        {
            double r = 0.0;
            //TODO: calculate the cov between adjacent regions.
            double cov_xy = 0.0;
            double cov_xx = 0.0;
            double cov_yy = 0.0;

            double ave_x = 0.0;
            double ave_y = 0.0;

            int x_start = x - (width - 1) / 2;
            int y_start = y - (width - 1) / 2;
            int x_end = x + (width - 1) / 2;
            int y_end = y + (width - 1) / 2;

            double sum_x = 0.0;
            double sum_y = 0.0;

            for (int i = x_start; i <= x_end; i++)
                for (int j = y_start; j <= y_end; j++)
                {
                    sum_x += result.GetPixel(i, j).R;
                    sum_y += result.GetPixel(i, j + width).R;
                }

            ave_x = sum_x / (width * width);
            ave_y = sum_y / (width * width);

            for (int i=x_start; i<=x_end; i++)
                for (int j = y_start; j < y_end; j++)
                {
                    cov_xx += (result.GetPixel(i, j).R - ave_x) * (result.GetPixel(i, j).R - ave_x);
                    cov_yy += (result.GetPixel(i, j + width).R - ave_y) * (result.GetPixel(i, j + 3).R - ave_y);
                    cov_xy += (result.GetPixel(i, j).R - ave_x) * (result.GetPixel(i, j + 3).R - ave_y);
                }

            cov_xx = cov_xx / (width * width - 1);
            cov_yy = cov_yy / (width * width - 1);
            cov_xy = cov_xy / (width * width - 1);

            r = cov_xy / Math.Sqrt(cov_xx * cov_yy);
            r = (255 - result.GetPixel(x, y).R) * Math.Abs(r);
            return r;
        }

        //After tumor's segmentation, exe this function. consider the n*n neighbourhood
        private Bitmap afterProcess(Bitmap result, int width, double percentage)
        {
            int result_width = result.Width;
            int result_height = result.Height;
            int[,] label = new int[result_height, result_width];

            for(int i = 0; i<result_height; i++)
                for (int j = 0; j < result_width; j++)
                {
                    Color result_color = result.GetPixel(i, j);
                    if (result_color.Name != "ff0000ff" )
                    {
                        if (label[i, j] != 1)
                            label[i, j] = 0;
                    }
                    else
                    { 
                        if (includeRedPixel(result, i, j, width, percentage))
                        {
                            for (int p = i - (width - 1) / 2; p <= i + (width - 1) / 2; p++)
                                for (int q = j - (width - 1) / 2; q <= j + (width - 1) / 2; q++)
                                {
                                    label[p, q] = 1;
                                }
                        }
                    }
                }

            for(int i=0; i<result_height; i++)
                for (int j = 0; j < result_width; j++)
                {
                    if (label[i, j] == 1)
                        result.SetPixel(i, j, Color.Yellow);
                }

            return result;
        }
        
        //Another function to do afterprocess, consider the line scan
        private Bitmap afterProcess(Bitmap result, int width)
        {
            int result_width = result.Width;
            int result_height = result.Height;
            int[,] label = new int[result_height, result_width];

            int farhest = 15;
            int start = 0;
            int end = 0;
            int continue_count = 0;

            for (int i = 0; i < result_height; i++)
            {
                start = 0;
                end = 0;
                continue_count = 0;
                for (int j = 0; j < result_width; j++)
                {
                    Color result_color = result.GetPixel(i, j);
                    if (result_color.Name == "ff0000ff")
                    {
                        if (start == 0)
                        {
                            start = j;
                            end = j;
                        }
                        else
                        {
                            end = j;
                        }
                    }
                    else
                    {
                        continue_count = end - start;
                        int step_pre = 0;
                        int step_post = 0;
                        if (continue_count > width)
                        {
                            int continue_redpre = 0;
                            int continue_redpost = 0;
                            int step = 1;
                            while (step < farhest)
                            {
                                Color pre = result.GetPixel(i, start - step);
                                Color post = result.GetPixel(i, end + step);
                                if (pre.Name == "ffff0000" && continue_redpre == 0)
                                {
                                    continue_redpre = 1;
                                }
                                else if (pre.Name != "ffff0000" && continue_redpre == 1)
                                {
                                    continue_redpre = 2;
                                    step_pre = step;
                                }

                                if (post.Name == "ffff0000" && continue_redpost == 0)
                                {
                                    continue_redpost = 1;
                                }
                                else if (post.Name != "ffff0000" && continue_redpost == 1)
                                {
                                    continue_redpost = 2;
                                    step_post = step;
                                }

                                if (continue_redpost == 2 && continue_redpre == 2)
                                    break;
                                
                                step++;
                            }

                           
                        }
                        for (int s = start - step_pre; s <= end + step_post; s++)
                            label[i, s] = 1;
                        start = 0;
                        end = 0;
                    }

                }
            }


            for (int i = 0; i < result_height; i++)
            {
                start = 0;
                end = 0;
                continue_count = 0;
                for (int j = 0; j < result_width; j++)
                {
                    Color result_color = result.GetPixel(j, i);
                    if (result_color.Name == "ff0000ff")
                    {
                        if (start == 0)
                        {
                            start = j;
                            end = j;
                        }
                        else
                        {
                            end = j;
                        }
                    }
                    else
                    {
                        continue_count = end - start;
                        int step_pre = 0;
                        int step_post = 0;
                        if (continue_count > width)
                        {
                            int continue_redpre = 0;
                            int continue_redpost = 0;
                            int step = 1;
                            while (step < farhest)
                            {
                                Color pre = result.GetPixel(start - step, i);
                                Color post = result.GetPixel(end+step, i);
                                if (pre.Name == "ffff0000" && continue_redpre == 0)
                                {
                                    continue_redpre = 1;
                                }
                                else if (pre.Name != "ffff0000" && continue_redpre == 1)
                                {
                                    continue_redpre = 2;
                                    step_pre = step;
                                }

                                if (post.Name == "ffff0000" && continue_redpost == 0)
                                {
                                    continue_redpost = 1;
                                }
                                else if (post.Name != "ffff0000" && continue_redpost == 1)
                                {
                                    continue_redpost = 2;
                                    step_post = step;
                                }

                                if (continue_redpost == 2 && continue_redpre == 2)
                                    break;

                                step++;
                            }

                            
                        }
                        for (int s = start - step_pre; s <= end + step_post; s++)
                            label[s, i] = 1;
                        start = 0;
                        end = 0;
                    }

                }
            }
            int[,] final_label = new int[result_height, result_width];
            for(int i=1; i<result_height-1; i++)
                for (int j = 1; j < result_width-1; j++)
                {
                    int count = 0;
                    
                    for(int p=i-1; p<=i+1; p++)
                        for (int q = j - 1; q <= j + 1; q++)
                        {
                            if (label[p, q] == 1)
                                count++;
                        }

                    if (count > 4)
                    {
                        final_label[i, j] = 1;
                    }
                    
                }
            Bitmap final_result = this._afterPre;
            for(int i=0; i<result_height; i++)
                for (int j = 0; j < result_width; j++)
                {
                    if (final_label[i, j] == 1)
                        final_result.SetPixel(i, j, Color.Yellow);
                }

            return final_result;
        }

        //judge whether the neighbourhood contains the tumor pixels.
        private bool includeRedPixel(Bitmap result, int x, int y, int width, double percent)
        {
            int x_start = x - (width - 1) / 2;
            int x_end = x + (width - 1) / 2;
            int y_start = y - (width - 1) / 2;
            int y_end = y + (width - 1) / 2;
            int num_of_blue = 0;
            for(int i=x_start; i<= x_end; i++)
                for (int j = y_start; j <= y_end; j++)
                {
                    if (result.GetPixel(i, j).Name == "ff0000ff")
                        num_of_blue++;
                }

            if (num_of_blue > (int)width * width * percent)
                return true;
            return false;          
        }

        #endregion
   
    }
}
