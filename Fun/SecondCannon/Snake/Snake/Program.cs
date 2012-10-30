using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Snake
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line = sr.ReadLine();
            String[] numbers = line.Split(' ');
            int rows = int.Parse(numbers[0]);
            int cols = int.Parse(numbers[1]);
            int[,] maze = new int[rows, cols];
            for (int i = 0; i < rows; i++)
            {
                line = sr.ReadLine();
                numbers = line.Split(' ');
                for (int j = 0; j < cols; j++)
                {
                    maze[i,j] = int.Parse(numbers[j]);
                }
            }
            int result = GetMaxWorms(0, 0, maze, cols, rows);
        }

        static int GetMaxWorms(int ipos, int jpos, int[,] maze, int cols, int rows)
        {
            int result = 0;
            if (ipos == rows - 1 && jpos < cols - 1)
            {
                return maze[ipos, jpos] + GetMaxWorms(ipos, jpos + 1, maze, cols, rows);
            }
            else if(jpos == cols -1 && ipos < rows - 1)
            {
                return maze[ipos, jpos] + GetMaxWorms(ipos + 1, jpos, maze, cols, rows);
            }
            else if (ipos == rows - 1 && jpos == cols - 1)
            {
                return maze[ipos, jpos];
            }
            else
            {
                int right = maze[ipos, jpos] + GetMaxWorms(ipos, jpos + 1, maze, cols, rows);
                int down = maze[ipos, jpos] + GetMaxWorms(ipos + 1, jpos, maze, cols, rows);
                return right > down ? right : down;
            }
        }
    }
}
