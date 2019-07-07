using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Linq;
using System.Reflection;
using System.Security.Cryptography; 

namespace Fixer_Nix
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            DownloadArchive();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void DownloadArchive()
        {
            WebClient webClient = new WebClient();

            webClient.DownloadFileCompleted += new AsyncCompletedEventHandler(Completo);
            webClient.DownloadProgressChanged += new DownloadProgressChangedEventHandler(ProgressoFeito);

            string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
            webClient.DownloadFileAsync(new Uri("http://167.114.109.15/update_regular/Fixer/checkUpdate.xml"), @path+"/checkUpdate.xml");
        }

        private void ProgressoFeito(object sender, DownloadProgressChangedEventArgs e)
        {
            progressBar.Value = e.ProgressPercentage;

            var bytes = Convert.ToInt32(e.TotalBytesToReceive);
            var bytesReceive = Convert.ToInt32(e.BytesReceived);
            
            bytes /= 1024;
            bytesReceive /= 1024;

            label1.Text = String.Format("{0}/{1} KB", bytesReceive, bytes);
            label2.Text = String.Format("{0}%", progressBar.Value);
        }

        class Arquivos
        {
            #region Atributos
            private int codigo;
            private string file_path;
            private string hash;
            #endregion
 
            #region Propriedades
            public int Codigo
            {
                get { return codigo; }
                set { codigo = value; }
            }
         
            public string File_path
            {
                get { return file_path; }
                set { file_path = value; }
            }
         
 
            public string Hash
            {
                get { return hash; }
                set { hash = value; }
            }
            #endregion

            #region Metodos
            public static List<Arquivos> ListarArquivos()
            {
                List<Arquivos> arquivos = new List<Arquivos>();
                XElement xml = XElement.Load("checkUpdate.xml");
                foreach(XElement x in xml.Elements())
                {
                    Arquivos p = new Arquivos()
                    {
                        codigo = int.Parse(x.Attribute("codigo").Value),
                        file_path = x.Attribute("file_path").Value,
                        hash = x.Attribute("hash").Value
                    };
                    arquivos.Add(p);
                }
                return arquivos;
            }
            #endregion
        }

        private void Completo(object sender, AsyncCompletedEventArgs e)
        {
            Logger box = new Logger(rBox);

            if (e.Error != null)
                box.Printf(true, true, Color.Red,  e.Error.Message);
            else
                Analyse(); 
        }

        private bool CheckHash(string old_hash, string file_path)
        {
            // Passo 1: Criar o algoritimo de HASH
            MD5 myHash = new MD5CryptoServiceProvider();
            // Passo 2: Armazenar os dados do arquivo em um array de bytes
            FileStream file = new FileStream(file_path, FileMode.Open, FileAccess.Read);
            BinaryReader reader = new BinaryReader(file);
            // Passo 3: Chamar o metodo HashAlgorithm.ComputeHash
            myHash.ComputeHash(reader.ReadBytes((int)file.Length));
            // Passo 4: Obter o HASH do arquivo
            string new_hash = Convert.ToBase64String(myHash.Hash);
            file.Close();

            if (new_hash != old_hash)
                return false;
            else
                return true;
        }

        private void CompletedDownload(object sender, AsyncCompletedEventArgs e)
        {
            Logger box = new Logger(rBox);

            if (e.Error != null)
                box.Printf(true, true, Color.Red, e.Error.Message); //MessageBox.Show(e.Error.Message);
            else
                box.Printf(true, true, Color.Green, "Download de " + sender.ToString() + " concluido.");
        }

        private void Analyse()
        {
            List<Arquivos> a = new List<Arquivos>(), baixar = new List<Arquivos>();
            a = Arquivos.ListarArquivos();

            string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);

            foreach (Arquivos x in a)
            {
                if (!File.Exists(path + x.File_path))
                {
                    baixar.Add(x);
                    continue;
                }
                else
                {
                    if (!CheckHash(x.Hash, path + x.File_path))
                    {
                        baixar.Add(x);
                        File.Delete(@path + x.File_path);
                    }
                }
            }

            a.Clear();

            Logger box = new Logger(rBox);

            foreach (Arquivos x in baixar)
            {
                box.Printf(true, true, Color.Green, "Fazendo download de " + x.File_path);
                WebClient webClient = new WebClient();
                
                webClient.DownloadFileAsync(new Uri("http://167.114.109.15/update_regular/Fixer" + x.File_path), @path + x.File_path);
                webClient.DownloadProgressChanged += new DownloadProgressChangedEventHandler(ProgressoFeito);
                webClient.DownloadFileCompleted += new AsyncCompletedEventHandler(CompletedDownload);
            }

            box.Printf(true, true, Color.Green, "Fix terminado.");
            baixar.Clear();
            File.Delete(path + "/checkUpdate.xml");
        }

        private void rBox_TextChanged(object sender, EventArgs e)
        {

        }

        #region BOX_LOGGER
        public struct ArgumentWorker
        {
            private String[] m_Message;
            private Color[] m_Color;

            public Color[] Colors { get { return m_Color; } }
            public String[] Messages { get { return m_Message; } }

            public ArgumentWorker(String[] Message, Color[] Color)
            {
                m_Message = Message;
                m_Color = Color;
            }
        }

        public class Logger
        {
            public delegate void FormCrossThreading(ArgumentWorker arg);

            private RichTextBox TextLog;
            private FormCrossThreading dlgToCrossThreading;

            public Logger(RichTextBox textLog)
            {
                this.TextLog = textLog;
                this.dlgToCrossThreading = new FormCrossThreading(this.AppendText);
            }

            public void Printf(Boolean SkipLine, Boolean Time, Color Color, String Text, params object[] Arguments)
            {
                Printf(SkipLine, Time, Color, String.Format(Text, Arguments));
            }

            public void Printf(Boolean SkipLine, Boolean Time, Color Color, String Text)
            {
                String[] Messages = new String[2];
                Color[] Colors = new Color[2];
                if (Time)
                    Messages[0] = String.Format("[{0:D2}/{1:D2}/{2:D2} {3:D2}:{4:D2}:{5:D2}] ", DateTime.Now.Day, DateTime.Now.Month, DateTime.Now.Year, DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second);
                else
                    Messages[0] = String.Empty;
                Colors[0] = Color.Gray;
                Messages[1] = Text;
                Colors[1] = Color;
                if (SkipLine)
                    Messages[1] += "\n";
                this.AppendText(new ArgumentWorker(Messages, Colors));
            }

            private void AppendText(ArgumentWorker Arguments)
            {
                try
                {
                    if (this.TextLog.InvokeRequired)
                    {
                        this.TextLog.Invoke(this.dlgToCrossThreading, Arguments);
                    }
                    else
                    {
                        if (Arguments.Messages.Length == Arguments.Colors.Length)
                        {
                            for (int i = 0; i < Arguments.Messages.Length; i++)
                            {
                                this.TextLog.SelectionColor = Arguments.Colors[i];
                                this.TextLog.AppendText(Arguments.Messages[i]);
                            }
                        }
                        else
                        {
                            this.TextLog.SelectionColor = Color.Red;
                            this.TextLog.AppendText("Error code: 0001.\n");
                        }
                    }
                }
                catch// (Exception ex)
                {
                    //throw ex;
                    return;
                }
            }
        }
        #endregion
    }
}

