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
using System.Xml.Serialization;


namespace GuesserGame
{
    /// <summary>
    /// Interaction logic for ModifyWordDialog.xaml
    /// </summary>
    public partial class ModifyWordDialog : Window
    {
        public string WordToModify { get; set; }
        public string NewCategory { get; set; }
        public string NewDescription { get; set; }
        public string NewImagePath { get; set; }

        public ModifyWordDialog()
        {
            InitializeComponent();
            PopulateCategoriesComboBox();
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
        private void PopulateCategoriesComboBox()
        {
            List<string> existingCategories = GetExistingCategories();

            // add categories in comboBox
            foreach (string category in existingCategories)
            {
                newCategoryComboBox.Items.Add(category);
            }

            List<string> existingWords = GetExistingWords();
            foreach (string word in existingWords)
            {
                wordToModifyTextBox.Items.Add(word);
            }

            // add a new category
            newCategoryComboBox.Items.Add("Introduceți o nouă categorie...");
        }
        void okclick()
        {
            if (string.IsNullOrEmpty(wordToModifyTextBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți cuvântul de modificat.");
                return;
            }

            if (string.IsNullOrEmpty(newCategoryComboBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți categoria nouă.");
                return;
            }

            if (string.IsNullOrEmpty(newDescriptionTextBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți descrierea nouă.");
                return;
            }
            WordToModify = wordToModifyTextBox.Text;
            NewCategory = newCategoryComboBox.Text;
            NewDescription = newDescriptionTextBox.Text;
            NewImagePath = newImagePathTextBox.Text;

            DialogResult = true;
            Close();
        }
        void imageclick()
        {
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog();
            openFileDialog.Filter = "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png|All files (*.*)|*.*";

            if (openFileDialog.ShowDialog() == true)
            {
                string sourceFilePath = openFileDialog.FileName;

                try
                {
                    string resourcesDirectory = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources");

                    if (!Directory.Exists(resourcesDirectory))
                    {
                        Directory.CreateDirectory(resourcesDirectory);
                    }

                    string fileName = Path.GetFileName(sourceFilePath);

                    //create path to new location of file to resource directory
                    string destinationFilePath = Path.Combine(resourcesDirectory, fileName);

                    //copy file in resource directory of project
                    File.Copy(sourceFilePath, destinationFilePath, true);

                    //refresh text in "Image Path" with local path
                    string relativePath = Path.Combine("Resources", fileName);
                    newImagePathTextBox.Text = relativePath;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Eroare la selectarea imaginii: " + ex.Message);
                }
            }
        }

        private List<string> GetExistingCategories()
        {
            List<string> existingCategories = new List<string>();

            // read from .xml if it exists.
            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    List<Word> existingWords = (List<Word>)serializer.Deserialize(fileStream);
                    foreach (Word word in existingWords)
                    {
                        if (!existingCategories.Contains(word.Category))
                        {
                            existingCategories.Add(word.Category);
                        }
                    }
                }
            }

            return existingCategories;
        }
        private void OkButton_Click(object sender, RoutedEventArgs e)
        {
            okclick();
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void SelectImageButton_Click(object sender, RoutedEventArgs e)
        {
            imageclick();
        }
        

        private void wordToModifyTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
