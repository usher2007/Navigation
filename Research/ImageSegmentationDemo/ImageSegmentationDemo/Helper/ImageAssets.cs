using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace ImageSegmentationDemo.Helper
{
    public class ImageAssets
    {
        private DirectoryInfo DirInfo;
        private FileInfo[] Files;
        private string _assetPath;
        private List<string> _fileList;
        public List<string> FileList
        {
            get { return this._fileList; }
        }

        public ImageAssets(string assetPath)
        {
            this._assetPath = assetPath;
            this.setFileList();
        }

        public string RemoveFirstAsset()
        {
            string firstAsset;   
            try
            {
                firstAsset = _fileList[0];
                _fileList.RemoveAt(0);
            }
            catch (Exception ex)
            {
                return string.Empty;
            }
            return firstAsset;
        }

        public string GetFirstAsset()
        {
            try
            {
                return _fileList[0];
            }
            catch (Exception ex)
            {
                return string.Empty;
            }
        }
        private void setFileList()
        {
            DirInfo = new DirectoryInfo(_assetPath.Trim());
            Files = DirInfo.GetFiles("*", SearchOption.TopDirectoryOnly);
            if (Files == null || Files.Length <= 0)
                return;

            Regex filePattern = new Regex(@"\d+\.bmp");
            _fileList = Files.Where(x => filePattern.IsMatch(x.Name)).Select(x => x.FullName).ToList();
        }
    }

    
}

