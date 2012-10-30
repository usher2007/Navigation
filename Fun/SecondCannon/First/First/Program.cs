using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Collections;

namespace First
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line;
            while ((line = sr.ReadLine()) != null)
            {
                int n = int.Parse(line);
                line = sr.ReadLine();
                string[] numbers = line.Split(' ');
                List<int> numList = new List<int>(numbers.Length);
                List<int> numLarge = new List<int>();
                List<int> numSmall = new List<int>();
                for (int i = 0; i < numbers.Length; i++)
                {
                    numList.Add(int.Parse(numbers[i]));
                }
                numList.Sort();
                if (numList.Count % 2 == 0)
                {
                    int pos1 = numList.Count / 2;
                    int pos2 = pos1 - 1;
                    numLarge.Add(numList[pos1]);
                    numSmall.Add(numList[pos2]);
                    numList.RemoveAt(pos1);
                    numList.RemoveAt(pos2);
                    List<int> reverseList = numList;
                    reverseList = reverseList.OrderByDescending(x => x).ToList();
                    for (int i = 0; i < pos1 - 1; i++)
                    {
                        numLarge.Add(numList[i]);
                        numSmall.Add(reverseList[i]);
                    }
                }
                else
                {
                    int smallestNum = 0;
                    if (numList[0] == 0)
                    {
                        smallestNum = numList[1];
                        numList.RemoveAt(1);
                    }
                    else
                    {
                        smallestNum = numList[0];
                        numList.RemoveAt(0);
                    }
                    numLarge.Add(smallestNum);
                    List<int> reverseList = numList;
                    reverseList = reverseList.OrderByDescending(x => x).ToList();
                    for (int i = 0; i < numList.Count / 2; i++)
                    {
                        numLarge.Add(numList[i]);
                        numSmall.Add(reverseList[i]);
                    }
                }
                int a = 0;
                a++;
            }

            
        }
    }
}
