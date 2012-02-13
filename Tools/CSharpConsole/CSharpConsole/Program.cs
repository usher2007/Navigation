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

        }
    }
}
