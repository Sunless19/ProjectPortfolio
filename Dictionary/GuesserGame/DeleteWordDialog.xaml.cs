using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Serialization;

namespace GuesserGame
{
    
    public partial class DeleteWordDialog : Window
    {
        public string WordToDelete { get; private set; }

        public DeleteWordDialog()
        {
            InitializeComponent();
            getWords();
        }
        

        private void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            WordToDelete = wordToDeleteComboBox.Text;
            DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void wordToDeleteTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
        private List<string> GetExistingWords()
        {
            List<string> existingCategories = new List<string>();

            // If the xml exists then read from .xml
            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    List<Word> existingWords = (List<Word>)serializer.Deserialize(fileStream);
                    foreach (Word word in existingWords)
                    {
                        if (!existingCategories.Contains(word.WordText))
                        {
                            existingCategories.Add(word.WordText);
                        }
                    }
                }
            }

            return existingCategories;
        }
        void getWords()
        {
            List<string> existingCategories = GetExistingWords();
            foreach (string category in existingCategories)
            {
                wordToDeleteComboBox.Items.Add(category);
            }
        }
    }
}
