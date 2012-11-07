using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Travel
{
    class Program
    {
        static void Main(string[] args)
        {
            List<Record> records = new List<Record>();
            StreamReader sr = new StreamReader("Test.in");
            String line = sr.ReadLine();
            int n = int.Parse(line);
            for (int i = 0; i < n; i++)
            {
                line = sr.ReadLine();
                int orderCount = int.Parse(line);
                for (int j = 0; j < orderCount; j++)
                {
                    line = sr.ReadLine();
                    String[] numbers = line.Split(' ');
                    int numOfRooms = int.Parse(numbers[0]);
                    int beginDay = int.Parse(numbers[1]);
                    int endDay = beginDay + int.Parse(numbers[2]) - 1;
                    Record tmp = new Record();
                    tmp.roomCount = numOfRooms;
                    tmp.beginDay = beginDay;
                    tmp.endDay = endDay;
                    records.Add(tmp);
                }

                records = records.OrderBy(x => x.beginDay).ToList<Record>();
                mergeOrders(records);
                int result = records.Sum(x => x.roomCount);
                
            }
        }
        static void mergeOrders(List<Record> records)
        {
            bool continueMerge = true;
            while (continueMerge)
            {
                continueMerge = mergeOneTime(records);
            }
            return;
        }

        static bool mergeOneTime(List<Record> records)
        {
            bool couldMerge = false;
            for (int i = 0; i < records.Count; i++)
            {
                for (int j = i + 1; j < records.Count; j++)
                {
                    Record first = records[i];
                    Record second = records[j];
                    if (first.endDay < second.beginDay)
                    {
                        int maxRoom = first.roomCount > second.roomCount ? first.roomCount : second.roomCount;
                        int newEndDay = second.endDay;
                        records.RemoveAt(j);
                        records[i].endDay = newEndDay;
                        records[i].roomCount = maxRoom;
                        return true;
                    }
                }
            }
            return couldMerge;
        }
    }

    class Record
    {
        public int roomCount = 0;
        public int beginDay = 0;
        public int endDay = 0;
    }

    
}
