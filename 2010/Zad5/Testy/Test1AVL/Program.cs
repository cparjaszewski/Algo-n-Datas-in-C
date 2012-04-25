using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Test1AVL
{
    class Program
    {
        static void Main(string[] args)
        {
            System.Collections.ArrayList M = new System.Collections.ArrayList();
            string s = System.Console.ReadLine();
            int N = int.Parse(s);
            char c;
            Int64 L;
            int x;
            for(int i=0;i<N;i++) {
                s = System.Console.ReadLine();
                c = s[0];
                L = Int64.Parse(s.Substring(2));
                M.Sort();
                switch (c) { 
                    case 'I':
                        if (!M.Contains(L))
                        {
                            M.Insert(0, L);
                        }
                        break;
                    case 'D':
                        x = Array.BinarySearch(M.ToArray(), L);
                        if(x < 0) {
                             System.Console.WriteLine("BRAK");
                        } else {
                            System.Console.WriteLine("OK");
                            M.Remove(L);
                        }
                        break;
                    case 'U':
                        x = Array.BinarySearch(M.ToArray(), L);
                        if (x >= 0) {
                            System.Console.WriteLine(M[x]);
                        } else if (~x < 0) {
                            System.Console.WriteLine("BRAK");
                        } else if ((~x) >= M.ToArray().Length) 
                        {
                            System.Console.WriteLine("BRAK");
                        }
                        else {
                            System.Console.WriteLine(M[~x].ToString());
                        }
                        break;
                    case 'L':
                        x = Array.BinarySearch(M.ToArray(), L);
                        if (x >= 0)
                        {
                            System.Console.WriteLine(M[x]);
                        }
                        else if (~x <= 0)
                        {
                            System.Console.WriteLine("BRAK");
                        }
                        else if ((~x) > M.ToArray().Length)
                        {
                            System.Console.WriteLine("BRAK");
                        }
                        else
                        {
                            System.Console.WriteLine(M[~x-1].ToString());
                        }

                        break;
                }
            }
        }
    }
}
