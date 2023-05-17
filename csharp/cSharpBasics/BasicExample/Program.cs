using System.Text.RegularExpressions;

namespace BasicExample
{

    class Program
    {
        static void testIfStatement(int a, int b)
        {
            if (a > b)
            {
                Console.WriteLine("a is bigger than b");
            }
            else
            {
                Console.WriteLine("b is bigger than a");
            }
        }

        static void testLoopWhile(int a, int b)
        {
            int count = 0;
            while (a < b)
            {
                Console.WriteLine($"looping {count}");
                Console.WriteLine($"a is {a}");
                count++;
                a++;
            }
            Console.WriteLine("while loop ended");
        }

        static void testLoopRepeat(int a, int b)
        {
            int count = 0;
            do
            {
                Console.WriteLine($"looping {count}");
                Console.WriteLine($"a is {a}");
                count++;
                a++;
            } while (a < b);
            Console.WriteLine("do loop ended");
        }

        static void testForloop(int a, int b)
        {
            for (int idx = a; idx < b; idx++)
            {
                Console.WriteLine($"looping {idx}");
            }
            Console.WriteLine("for loop ended");
        }

        static void Main(string[] args)
        {

            sbyte sb = 10;
            byte by = 10;

            short ss = SByte.MaxValue + 1;
            ushort us = Byte.MaxValue + 1;

            int i = short.MaxValue + 1;
            uint ui = ushort.MaxValue + 1;

            long l = (long)int.MaxValue + 1;
            ulong ul = (ulong)uint.MaxValue + 1;

            float f = 3.324f;
            double d = 35438580.21473;

            bool b = false;
            char c = 'A';
            string str = "Hello there";

            // Basic data type
            Console.WriteLine("Basic data types");
            Console.WriteLine("~~~~~~~~~~~~~~~~");
            Console.WriteLine("Integers");
            Console.WriteLine("~~~~~~~~~");
            Console.WriteLine($"Byte value: {by}");
            Console.WriteLine($"signed byte value: {sb}");
            Console.WriteLine($"unsigned short value: {us}");
            Console.WriteLine($"signed short value: {ss}");
            Console.WriteLine($"unsigned int: {ui}");
            Console.WriteLine($"signed int: {i}");
            Console.WriteLine($"unsigned long int: {ul}");
            Console.WriteLine($"signed long: {l}");
            Console.WriteLine("~~~~~~~~~");
            Console.WriteLine("Floating points");
            Console.WriteLine("~~~~~~~~~~~~~~~");
            Console.WriteLine($"float: {f}");
            Console.WriteLine($"double: {d}");
            Console.WriteLine("~~~~~~~~~~~~~~~");
            Console.WriteLine("String and characters");
            Console.WriteLine($"Basic character: {c}");
            Console.WriteLine($"Basic string: {str}");
            Console.WriteLine("~~~~~~~~~~~~~~~~");
            Console.WriteLine("Boolean");
            Console.WriteLine($"boolean example: {b}");
            Console.WriteLine("~~~~~~~~~~~~~~~~");

            // If testing
            int noA = 78;
            int noB = 77;
            testIfStatement(noA, noB);
            noB = 79;
            testIfStatement(noA, noB);
            noA = 2;
            noB = 10;

            // Loop testing
            Console.WriteLine("while loop test");
            Console.WriteLine("~~~~~~~~~~~~~~~");
            testLoopWhile(noA, noB);
            Console.WriteLine("do while test");
            Console.WriteLine("~~~~~~~~~~~~~");
            testLoopRepeat(noA, noB);
            Console.WriteLine("for test");
            Console.WriteLine("~~~~~~~~");
            testForloop(noA, noB);


            Console.WriteLine("input test");
            Console.WriteLine("~~~~~~~~~~");

            Console.WriteLine("Please provide a number");
            string input = Console.ReadLine();
            int intVal = 0;

            while (!int.TryParse(input, out intVal))
            {
                Console.WriteLine("Error");
                input = Console.ReadLine();
            };

            Console.WriteLine($"Entered number : {intVal}");
            Console.WriteLine("Please provide a string in small letters");
            input = Console.ReadLine();
            var regex = new Regex("^[a-z]+$");
            while (!regex.IsMatch(input))
            {
                Console.WriteLine("Error");
                input = Console.ReadLine();
            }
            Console.WriteLine($"Entered string : {input}");

            // Collection usage - also for each
            Console.WriteLine("collection test");
            Console.WriteLine("~~~~~~~~~~");
            List<int> collection = new List<int>();
            Console.WriteLine($"Collection contains {collection.Count} elements");
            collection.Add(6);
            collection.Add(7);
            collection.Add(8);
            collection.Add(9);
            Console.WriteLine($"Collection contains {collection.Count} elements");
            int extracted = collection[collection.Count - 1];

            collection.Remove(9);
            Console.WriteLine($"Extracted is {extracted}");
            Console.WriteLine($"Collection contains {collection.Count} elements");
            Console.WriteLine($"Collection contains 8 : {collection.Find(val => val == 8)}");
            foreach (int idx in collection)
            {
                Console.WriteLine($"{idx}");
            }

            ExampleStruct exampleStruct = new ExampleStruct();
            exampleStruct.a = 10;
            exampleStruct.b = "Hello";

            ExampleStruct exampleStruct2 = new ExampleStruct();

            Console.WriteLine(exampleStruct);
            Console.WriteLine(exampleStruct.a);
            Console.WriteLine(exampleStruct.b);
            bool compare = exampleStruct.Equals(exampleStruct2);
            Console.WriteLine($"Comparing Results : {compare}");
        }

    }
}