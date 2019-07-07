using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Linq;
using System.Reflection;
using System.Security.Cryptography; 
using System.IO;

namespace Gerador_XML
{
    class Program
    {
       static void Listar()
        {
            XElement xml = XElement.Load("checkUpdate.xml");

            int i = 0;

            string path = Path.GetDirectoryName(Assembly.GetExecutingAssembly().GetModules()[0].FullyQualifiedName);
            DirectoryInfo Dir = new DirectoryInfo(@path);
            // Busca automaticamente todos os arquivos em todos os subdiretórios
            FileInfo[] Files = Dir.GetFiles("*", SearchOption.AllDirectories);
            foreach (FileInfo File in Files)
            {
                i++;
                // Retira o diretório iformado inicialmente
                string FileName = File.FullName.Replace(Dir.FullName, "");

                MD5 myHash = new MD5CryptoServiceProvider();
                // Passo 2: Armazenar os dados do arquivo em um array de bytes
                FileStream file = new FileStream(path + FileName, FileMode.Open, FileAccess.Read);
                BinaryReader reader = new BinaryReader(file);
                // Passo 3: Chamar o metodo HashAlgorithm.ComputeHash
                myHash.ComputeHash(reader.ReadBytes((int)file.Length));
                // Passo 4: Obter o HASH do arquivo
                string new_hash = Convert.ToBase64String(myHash.Hash);

                XElement x = new XElement("arquivo");
                x.Add(new XAttribute("codigo", i.ToString()));
                x.Add(new XAttribute("file_path", FileName));
                x.Add(new XAttribute("hash", new_hash));
                xml.Add(x);
            }

            xml.Save("checkUpdate.xml");
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
               foreach (XElement x in xml.Elements())
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

        static void Main(string[] args)
        {
            Listar();
            Console.WriteLine("Processo terminado.");
            Console.ReadKey();
        }
    }
}
