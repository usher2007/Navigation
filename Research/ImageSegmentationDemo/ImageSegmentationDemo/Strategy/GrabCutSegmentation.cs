using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Emgu.CV.Structure;
using Emgu.CV;

namespace ImageSegmentationDemo.Strategy
{
    public class GrabCutSegmentation : ISegmentation
    {
        private Image<Bgr, Byte> _totalImage;
        private Image<Gray, Byte> _mask;
        private static int ITERATIONCOUNT = 4;
        public System.Drawing.Bitmap Segment(System.Drawing.Bitmap image, Image<Bgr, Byte> originalImage, System.Drawing.Rectangle rect)
        {
            _totalImage = new Image<Bgr, byte>(image);
            _mask = _totalImage.GrabCut(rect, ITERATIONCOUNT);
            CvInvoke.cvAndS(_mask.Ptr, new MCvScalar(1d), _mask.Ptr, IntPtr.Zero);
            Image<Bgr, Byte> result = originalImage.Copy(_mask);
            return result.ToBitmap();
        }
    }
}
