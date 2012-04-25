using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestGenerator
{
    class Program
    {
        static Random rand1;
        static Random rand2; 

        static void Main(string[] args)
        {
            int A = 30000, B = 80000, C = 10000;
            int N = 3 * A + 3 * B + 9 * C;
            System.Console.WriteLine(N.ToString());
            rand1 = new Random(-1000000000);
            rand2  = new Random(1000000000);
            
            Int64 tmp = 1000000000000000000;
            
            for (int i = 0; i < A; i++) {
                tmp = getNextRand();
                System.Console.WriteLine("I " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("U " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("L " + tmp.ToString());
            }
           
            for (int i = 0; i < B; i++)
            {
                tmp = getNextRand();
                System.Console.WriteLine("D " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("U " + tmp.ToString());
                tmp = getNextRand(); ;
                System.Console.WriteLine("L " + tmp.ToString());
            }
            for (int i = 0; i < C; i++)
            {
                tmp = getNextRand();
                System.Console.WriteLine("I " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("I " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("L " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("U " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("D " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("D " + tmp.ToString());
                tmp = getNextRand(); ;
                System.Console.WriteLine("I " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("D " + tmp.ToString());
                tmp = getNextRand();
                System.Console.WriteLine("L " + tmp.ToString());
            }
        }

        static Int64 getNextRand() {
            return (((Int64)(rand2.Next())) * 10 + rand1.Next()) / 100000000;
        }
    }
}

