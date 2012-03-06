using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSharpConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            HashSet<Int32> set1 = new HashSet<Int32>(){1,2,3};
            HashSet<Int32> set2 = new HashSet<Int32>() { 2, 3, 4 };
            set1.UnionWith(set2);

            Int32 type = (Int32)Type.Default;
            Type t = (Type)5;
            bool b = t.Equals(Type.Default);
            HashSet<string> freqWords = new HashSet<string>() { "U.S.", "Oct.", "Jan.", "Nov.", "Dec.", "Sept.", "Aug.", "Dr.", "Feb.", "Sen.", "Gov.", "Rep.", "vs.", "Mr.", "St.", "Jr.", "No.", "L.A.", "D.C.", "Gen.", "Mrs." };
            List<String> strList = new List<String>() { "abc", "bdc", "abd", "abc", "bdc" };
            HashSet<string> strHash = new HashSet<string>(strList);
            string w = "Dr.";
            bool has = freqWords.Contains(w);
            List<Int32> keys = new List<Int32>();
            Dictionary<Int32, Int32> keyValueCollection = new Dictionary<Int32, Int32>();
            keyValueCollection.Add(1, 1);
            keys.Add(1);
            keyValueCollection.Add(2, 2);
            keys.Add(2);
            keyValueCollection.Add(3, 3);
            keys.Add(3);
            keyValueCollection.Add(4, 4);
            keys.Add(4);

            Int32 removedKey = keys.First();
            keyValueCollection.Add(5, 5);
            keys.Add(5);
            keyValueCollection.Remove(removedKey);
            keys.Remove(removedKey);

            removedKey = keys.First();
            keyValueCollection.Add(6, 6);
            keys.Add(5);
            keyValueCollection.Remove(removedKey);
            keys.Remove(removedKey);

            keyValueCollection[7] = 9;
            keyValueCollection[8] = 10;

            String path = "D:\\data\\sqldump\\description.csv";
            String outPath = "D:\\scripts\\shortDescription.tsv";
            System.IO.StreamReader lineReader = new System.IO.StreamReader(path);
            System.IO.StreamWriter lineWriter = new System.IO.StreamWriter(outPath);
            String testStr = "";
            int total_line = 0;
            while ((testStr = lineReader.ReadLine()) != null)
            {
                total_line++;
                if (total_line == 71)
                {
                    total_line = total_line;
                }
                testStr = testStr.Trim("\"".ToCharArray());
                string trimedDescription = "";
                if (testStr.Length < 120)
                {
                    trimedDescription = testStr;
                }
                else
                {
                    string subText = testStr.Substring(0, 120);
                    int lastSpaceIndex = subText.Substring(0, 40).LastIndexOf(' ');
                    int trimIndex = subText.LastIndexOf(' ');
                    string currentWord = "";
                    trimedDescription = subText.Substring(0, trimIndex) + "...";
                    char[] trimChars = { '\'', '\"' };
                    for (int index = 40; index < subText.Length; index++)
                    {
                        if (subText[index] == ' ')
                        {
                            currentWord = subText.Substring(lastSpaceIndex + 1, index - lastSpaceIndex - 1);
                            if (subText[index - 1] == '.' && !freqWords.Contains(currentWord.Trim(trimChars)))
                            {
                                char nextHead = ' ';
                                int subIndex = index + 1;
                                while (subIndex < subText.Length && subText[subIndex] == ' ')
                                {
                                    subIndex++;
                                }
                                bool reachEnd = false;
                                if (subIndex >= subText.Length)
                                {
                                    reachEnd = true;
                                }
                                else
                                {
                                    nextHead = subText[subIndex];
                                }
                                bool isThisSentenceEnd = char.IsUpper(nextHead);
                                if (isThisSentenceEnd || reachEnd)
                                {
                                    trimedDescription = subText.Substring(0, index);
                                }
                            }
                            lastSpaceIndex = index;
                        }
                    }
                    
                }
                lineWriter.WriteLine(trimedDescription + '\t' + testStr);
            }

        }

        public enum Type
        {
            Default = 0,
            Video = 1,
            Movie = 2,
            Show = 3,
        };
    }

}
