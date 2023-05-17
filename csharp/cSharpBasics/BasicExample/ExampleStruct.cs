using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BasicExample
{ 
    public struct ExampleStruct
    {
        public int a { get; set; }
        public string b { get; set; }
        public ExampleStruct()
        {
            this.a = 0;
            this.b = "";
        }
    };
}
