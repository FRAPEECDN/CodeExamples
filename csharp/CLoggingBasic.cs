using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;


namespace Example
{
    // Basic loggin class, will use similar ability than MFC code to retrive what to log
    // Category (Name for what will be logged)
    // Path - Log file location
    // GeneralLogging - contains logging level ( 0 - errors, 1 function high level operation points, 2 - everything )
    // LoggingSize - how much info can be logged
    // LogFileDeletes - indicate if log should be cleared before running
    class CLogginBasic
    {
        // Since we wan't to use similar code as legacy base, we load this API calls directly for usage
        [DllImport("kernel32", EntryPoint = "GetPrivateProfileStringA")]
        private static extern int GetPrivateProfileString(string Section, string Key, string Default, StringBuilder RetVal, int Size, string FilePath);
        [DllImport("kernel32", EntryPoint = "GetPrivateProfileIntA")]
        private static extern int GetPrivateProfileInt(string Section, string Key, int Default, string FilePath);

        private const int _MAX_PATH = 260;
        private int m_LogLevel = 0;
        private int m_bLogSize = 0;

        private string m_sLogPath = "";
        private string m_sINIPath = "";
        private string m_sCategory = "";
        private StreamWriter m_Writer;

        public bool LogActive { get; private set; }

        // Constructor - starts the log
        public CLogginBasic()
        {
            int retLog = 0;
            StringBuilder ret = new StringBuilder();
            string path, sDefaultPath;

            path = Path.GetDirectoryName(System.Reflection.Assembly.GetAssembly(this.GetType()).Location);
            m_sINIPath = path + "\\ApplicationInfo.txt";
            path = Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData) + "\\ApplicationName";
            sDefaultPath = path + "\\GeneralLog.log";

            m_LogLevel = GetPrivateProfileInt("VERSION", "GeneralLogging", 0, m_sINIPath);
            LogActive = (m_LogLevel != 0);
            ret.Clear();
            GetPrivateProfileString("VERSION", "LoggingPath", sDefaultPath, ret, _MAX_PATH, m_sINIPath);
            m_sLogPath = ret.ToString();
            ret.Clear();
            GetPrivateProfileString("VERSION", "LoggingCategory", "*", ret, 1024, m_sINIPath);
            m_sCategory = ret.ToString();

            m_bLogSize = GetPrivateProfileInt("VERSION", "LoggingSize", 10000000, m_sINIPath);
            retLog = GetPrivateProfileInt("VERSION", "LogFileDeletes", 0, m_sINIPath);
            bool blnDeleteLogFile = (retLog == 1);
            if (blnDeleteLogFile)
            {
                System.IO.File.Delete(m_sLogPath);
            }

            if (!System.IO.Directory.Exists(Path.GetDirectoryName(m_sLogPath)))
            {
                System.IO.Directory.CreateDirectory(Path.GetDirectoryName(m_sLogPath));
            }

            try
            {
                m_Writer = new StreamWriter(m_sLogPath, true);
            }
            catch (IOException e)
            {
                LogActive = false;
                System.Diagnostics.Debug.WriteLine("Exception Message: " + e.Message);
            }
        }

        public void WriteLog(string Category, int verbosity, string comment)
        {
            if (!LogActive && (verbosity < m_LogLevel))
                return;

            bool Proceed = ((m_sCategory == "*") || (m_sCategory.IndexOf(Category, 0) >= 0));
            if (Proceed)
            {
                string OutMessage;
                DateTime timestamp = DateTime.Now;
                // The basic log message, timestamp - machine - user - category - and then what is being logged
                OutMessage = String.Format("({0}) {1}:{2} ____ {3} := {4}", timestamp.ToString(), System.Environment.MachineName, System.Environment.UserName, Category, comment);

                if (m_Writer == null)
                {
                    try
                    {
                        m_Writer = new StreamWriter(m_sLogPath, true);
                    }
                    catch (IOException e)
                    {
                        LogActive = false;
                        System.Diagnostics.Debug.WriteLine("Exception Message: " + e.Message);
                    }
                }

                if (m_Writer != null)
                {
                    m_Writer.WriteLine(OutMessage);
                    m_Writer.Flush();
                }
            }
        }

        // Allows final log file entry to be written, to make sure log is complete
        public void FinalWrite()
        {
            if (m_Writer != null)
            {
                m_Writer.WriteLine("++++++++++++++++END++++++++++++++++");
                m_Writer.WriteLine("LOG DONE FROM EXAMPLE");
                m_Writer.WriteLine("++++++++++++++++END++++++++++++++++");
                m_Writer.Close();
            }
        }
    }
}
