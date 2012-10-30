using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Typer
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamReader sr = new StreamReader("Test.in");
            String line = sr.ReadLine();
            String[] numbers = line.Split(' ');
            int[] numOri = new int[6];
            int[] numDest = new int[6];
            for (int i = 0; i < 6; i++)
            {
                numOri[i] = int.Parse(numbers[0][i].ToString());
                numDest[i] = int.Parse(numbers[1][i].ToString());
            }
            Node origin = new Node();
            copy(numOri, origin.num);
            origin.step = 0;
            origin.cursorIndex = 0;
            Queue<Node> nodeQueue = new Queue<Node>();
            nodeQueue.Enqueue(origin);
            int finalStep = 0;
            int[] flag = new int[7000000];
            int flagNum = toNum(origin.num, origin.cursorIndex);
            flag[flagNum] = 1;
            while (nodeQueue.Count > 0)
            {
                Node tmp = nodeQueue.Dequeue();
                if (check(tmp.num, numDest) == true)
                {
                    finalStep = tmp.step;
                    break;
                }
                if (tmp.cursorIndex < 5 && tmp.behave != 1)
                {
                    Node right = new Node();
                    copy(tmp.num, right.num);
                    right.cursorIndex = tmp.cursorIndex + 1;
                    right.step = tmp.step + 1;
                    right.behave = 0;
                    flagNum = toNum(right.num, right.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(right);
                        flag[flagNum] = 1;
                    }
                }

                if (tmp.cursorIndex > 0 && tmp.behave != 0)
                {
                    Node left = new Node();
                    copy(tmp.num, left.num);
                    left.cursorIndex = tmp.cursorIndex - 1;
                    left.step = tmp.step + 1;
                    left.behave = 1;
                    flagNum = toNum(left.num, left.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(left);
                        flag[flagNum] = 1;
                    }
                }

                if (tmp.num[tmp.cursorIndex] < 9 && tmp.behave != 3)
                {
                    Node up = new Node();
                    copy(tmp.num, up.num);
                    up.cursorIndex = tmp.cursorIndex;
                    up.num[up.cursorIndex]++;
                    up.step = tmp.step + 1;
                    up.behave = 2;
                    flagNum = toNum(up.num, up.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(up);
                        flag[flagNum] = 1;
                    }
                }

                if (tmp.num[tmp.cursorIndex] > 0 && tmp.behave != 2)
                {
                    Node down = new Node();
                    copy(tmp.num, down.num);
                    down.cursorIndex = tmp.cursorIndex;
                    down.num[down.cursorIndex]--;
                    down.step = tmp.step + 1;
                    down.behave = 3;
                    flagNum = toNum(down.num, down.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(down);
                        flag[flagNum] = 1;
                    }
                }

                if (tmp.num[tmp.cursorIndex] != tmp.num[5] && tmp.behave != 4)
                {
                    Node swapEnd = new Node();
                    copy(tmp.num, swapEnd.num);
                    swapEnd.cursorIndex = tmp.cursorIndex;
                    int tmpNum = swapEnd.num[swapEnd.cursorIndex];
                    swapEnd.num[swapEnd.cursorIndex] = swapEnd.num[5];
                    swapEnd.num[5] = tmpNum;
                    swapEnd.step = tmp.step + 1;
                    swapEnd.behave = 4;
                    flagNum = toNum(swapEnd.num, swapEnd.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(swapEnd);
                        flag[flagNum] = 1;
                    }
                }

                if (tmp.num[tmp.cursorIndex] != tmp.num[0] && tmp.behave != 5)
                {
                    Node swapBegin = new Node();
                    copy(tmp.num, swapBegin.num);
                    swapBegin.cursorIndex = tmp.cursorIndex;
                    int tmpNum = swapBegin.num[swapBegin.cursorIndex];
                    swapBegin.num[swapBegin.cursorIndex] = swapBegin.num[0];
                    swapBegin.num[0] = tmpNum;
                    swapBegin.step = tmp.step + 1;
                    swapBegin.behave = 5;
                    flagNum = toNum(swapBegin.num, swapBegin.cursorIndex);
                    if (flag[flagNum] == 0)
                    {
                        nodeQueue.Enqueue(swapBegin);
                        flag[flagNum] = 1;
                    }
                }
            }

            finalStep = finalStep;
            
        }

        static bool check(int[] numCur, int[] numDest)
        {
            for (int i = 0; i < 6; i++)
            {
                if (numCur[i] != numDest[i])
                {
                    return false;
                }
            }
            return true;
        }
        static void copy(int[] numOri, int[] numTmp)
        {
            for (int i = 0; i < 6; i++)
            {
                numTmp[i] = numOri[i];
            }
            return;
        }

        static int toNum(int[] num, int cursor)
        {
            int result = 0;
            for (int i = 0; i < 6; i++)
            {
                result = result * 10 + num[i];
            }
            result = result + cursor * 1000000;
            return result;
        }
    }

    class Node
    {
        public int[] num = new int[6];
        public int cursorIndex = 0;
        public int step = 0;
        public int behave = -1;//0 right 1 left 2 up 3 down 4 swapEnd 5 swapBegin
    }
}
