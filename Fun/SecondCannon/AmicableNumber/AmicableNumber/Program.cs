using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AmicableNumber
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line;
            line = sr.ReadLine();
            int n = int.Parse(line);
            for (int i = 0; i < n; i++)
            {
                line = sr.ReadLine();
                string[] numbers = line.Split(' ');
                int first = int.Parse(numbers[0]);
                int second = int.Parse(numbers[1]);
                int firstSum = getSumOfApproNum(first);
                int secondSum = getSumOfApproNum(second);
                if (firstSum == second && secondSum == first)
                {
                    int a = 0;
                }
                else
                {
                    int b = 0;
                }
            }
        }

        static int getSumOfApproNum(int num)
        {
            int sum = 0;
            for (int i = 1; i < num; i++)
            {
                if (num % i == 0)
                {
                    sum += i;
                }
            }
            return sum;
        }
    }
}
