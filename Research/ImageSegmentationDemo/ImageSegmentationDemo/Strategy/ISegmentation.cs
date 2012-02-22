using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Emgu.CV.Structure;
using Emgu.CV;

namespace ImageSegmentationDemo.Strategy
{
    public interface ISegmentation
    {
        Bitmap Segment(Bitmap image, Image<Bgr, Byte> originalImage, Rectangle rect);
    }
}
