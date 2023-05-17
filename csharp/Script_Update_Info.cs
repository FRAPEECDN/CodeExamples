using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;

namespace ChangeASVersion
{
    class Program
    {
        // Test if a specific bath is valid (aka Directory actually exists)
        static bool IsValidPath(string path, out string UsePath, bool allowRelativePaths = false)
        {
            bool isValid = true;
            UsePath = path;

            try
            {
                string fullPath = Path.GetFullPath(path);

                if (allowRelativePaths)
                {
                    isValid = Path.IsPathRooted(fullPath);
                    if (isValid) UsePath = fullPath;
                }
                else
                {
                    string root = Path.GetPathRoot(path);
                    isValid = string.IsNullOrEmpty(root.Trim(new char[] { '\\', '/' })) == false;
                }

                // Now test the extension path to make sure it is a directory
                isValid &= Directory.Exists(path);
            }
            catch (Exception exp)
            {
                string msg;

                msg = String.Format(CultureInfo.InvariantCulture, "Problem detected with script - {0} - {1}", exp.HResult, exp.Message);
                isValid = false;
            }

            return isValid;
        }
        // Test the file path to make sure it is valid (includes actual file), support relative paths as well
        static bool IsValidPathWithFile(string path, out string UsePath, bool allowRelativePaths = false)
        {
            bool isValid = true;
            UsePath = path;

            try
            {
                string fullPath = Path.GetFullPath(path);

                if (allowRelativePaths)
                {
                    isValid = Path.IsPathRooted(fullPath);
                    if (isValid) UsePath = fullPath;
                }
                else
                {
                    string root = Path.GetPathRoot(path);
                    isValid = string.IsNullOrEmpty(root.Trim(new char[] { '\\', '/' })) == false;
                }

                // Now test the actual file (split name and extension)
                string extension = Path.GetExtension(path);
                string filename = Path.GetFileNameWithoutExtension(path);
                isValid &= (filename.Equals("AssemblyInfo")); // Make sure the file is correct 
                isValid &= (extension.Equals(".cs")); // Make sure extension is correct
                isValid &= (File.Exists(UsePath));
            }
            catch (Exception exp)
            {
                string msg;

                msg = String.Format(CultureInfo.InvariantCulture, "Problem detected with script - {0} - {1}", exp.HResult, exp.Message);
                isValid = false;
            }

            return isValid;
        }

        // Scan from a starting path and change the file extension with the versions
        // extension should be something like "*.url"
        static bool ScanAndChangeFiles(string startPath, string extension, int oldVersion, int newVersion)
        {
            // create an array of all files containing the extension since that is the files that needs updates
            string[] files = Directory.GetFiles(startPath,  extension, SearchOption.AllDirectories);
            if (files.Count() == 0) return false;

            foreach (string fs in files)
            {
                if (!ScanFile(fs, oldVersion, newVersion))
                    return false;
            }
            return true;
        }

        static bool VersionChange(string InPutFile, Version VersionNo)
        {
            // Create a temp file which will received the changed version information
            string tempPath = Path.GetTempFileName();

            // Read from the input file
            using (StreamReader reader = new StreamReader(InPutFile, System.Text.Encoding.Default))
            {
                // Then write to temp file
                using (StreamWriter writer = new StreamWriter(tempPath, false, reader.CurrentEncoding))
                {
                    string line;

                    while ((line = reader.ReadLine()) != null)
                    {
                        // Detect the file content that needs the version to be upgraded
                        if (line.Contains("[assembly: AssemblyCopyright("))
                        {
                            string linespacing = line.Substring(0, line.IndexOf("[assembly: AssemblyCopyright("));
                            line = string.Format(CultureInfo.InvariantCulture, "{1}[assembly: AssemblyCopyright(\"Copyright © CaseWare International Inc. {0}\")]", VersionNo.Major, linespacing);
                        }
                        else if (line.Contains("[assembly: AssemblyVersion("))
                        {
                            string linespacing = line.Substring(0, line.IndexOf("[assembly: AssemblyVersion("));
                            line = string.Format(CultureInfo.InvariantCulture, "{4}[assembly: AssemblyVersion(\"{0}.{1}.{2}.{3}\")]", VersionNo.Major, VersionNo.Minor, VersionNo.Build, VersionNo.Revision, linespacing);
                        }
                        else if (line.Contains("[assembly: AssemblyFileVersion("))
                        {
                            string linespacing = line.Substring(0, line.IndexOf("[assembly: AssemblyFileVersion("));
                            line = string.Format(CultureInfo.InvariantCulture, "{4}[assembly: AssemblyFileVersion(\"{0}.{1}.{2}.{3}\")]", VersionNo.Major, VersionNo.Minor, VersionNo.Build, VersionNo.Revision, linespacing);
                        }

                        try
                        {
                            writer.WriteLine(line);
                        }
                        catch (IOException exp)
                        {
                            string msg;

                            msg = String.Format(CultureInfo.InvariantCulture, "Problem detected with script - {0} - {1}", exp.HResult, exp.Message);
                            Console.WriteLine(msg);
                            return false;
                        }
                    }
                }

            }
            try
            {
                File.Copy(tempPath, InPutFile, true);
            }
            catch (Exception exp)
            {
                string msg;

                msg = String.Format(CultureInfo.InvariantCulture, "Problem detected with script - {0} - {1}", exp.HResult, exp.Message);
                Console.WriteLine(msg);
                return false;
            }
            return true;
        }

        // Allows command line to provide parameters for the upgrade (path to file and version number)
        static void Main(string[] args)
        {
            int MayorVersion = 0;
            string ValidFileName;

            if (args.Count() != 2)
            {
                Console.WriteLine("Not enough arguments");
                return;
            }

            if (!Program.IsValidPathWithFile(args[0], out ValidFileName, true))
            {
                Console.WriteLine("Invalid path reference");
                return;
            }

            if (!Int32.TryParse(args[1], out MayorVersion))
            {
                Console.WriteLine("Version number provided is not integer");
                return;
            }
            Version ver = new Version(MayorVersion, 0, 1, 0);

            if (VersionChange(ValidFileName, ver))
            {
                string msg;

                msg = String.Format(CultureInfo.InvariantCulture, "File version done for - {0}", ValidFileName);
                Console.WriteLine(msg);
            }
            else
            {
                string msg;

                msg = String.Format(CultureInfo.InvariantCulture, "Could not convert file version for - {0}", ValidFileName);
                Console.WriteLine(msg);
            }

            //Console.ReadLine(); // Do nothing except allow user to see outPut (for debug session)
        }
    }
}
