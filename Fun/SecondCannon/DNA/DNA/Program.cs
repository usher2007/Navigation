using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace DNA
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line = sr.ReadLine();
            String[] numbers = line.Split(' ');
            int len = int.Parse(numbers[0]);
            int inputCount = int.Parse(numbers[1]);
            SortedDictionary<int, String> inverNumToDNA = new SortedDictionary<int, String>();
            for (int i = 0; i < inputCount; i++)
            {
                line = sr.ReadLine();
                inverNumToDNA.Add(numOfInverse(line, len), line);
            }
            
            int a = 0;
        }

        static int numOfInverse(String dna, int len)
        {
            int result = 0;
            for (int i = 0; i < len; i++)
            {
                char current = dna[i];
                for (int j = i + 1; j < len; j++)
                {
                    if (current > dna[j])
                    {
                        result++;
                    }
                }
            }
            return result;
        }
    }
}
