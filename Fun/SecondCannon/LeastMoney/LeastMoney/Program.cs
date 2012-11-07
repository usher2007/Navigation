using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace LeastMoney
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line;
            int[] money = {100, 50, 20, 10, 5, 2, 1};
            while ((line = sr.ReadLine()) != null)
            {
                int n = int.Parse(line);
                if (n != 0)
                {
                    int sum = 0;
                    int remain = n;
                    for (int i = 0; i < money.Length; i++)
                    {
                        sum += (remain / money[i]);
                        remain = (remain % money[i]);
                    }
                    int a = 0;
                    a++;
                }
            }
        }
    }
}
