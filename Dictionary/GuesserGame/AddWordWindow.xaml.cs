
using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Xml.Serialization;

namespace GuesserGame
{
    public partial class AddWordWindow : Window
    {
        public AddWordWindow()
        {
            InitializeComponent();
            addWordsToComboBox();
        }
        private void SelectImageButton_Click(object sender, RoutedEventArgs e)
        {
            imageClick();
        }
        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            addWord();
        }

        private void imagePathTextBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
        {

        }

        private void categoryComboBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {

        }

        void addWordsToComboBox()
        {
            List<string> existingCategories = GetExistingCategories();
            foreach (string category in existingCategories)
            {
                categoryComboBox.Items.Add(category);
            }
        }
        private List<string> GetExistingCategories()
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
                        if (!existingCategories.Contains(word.Category))
                        {
                            existingCategories.Add(word.Category);
                        }
                    }
                }
            }

            return existingCategories;
        }

        void imageClick()
        {
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog();
            openFileDialog.Filter = "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png|All files (*.*)|*.*";

            if (openFileDialog.ShowDialog() == true)
            {
                // path to selected folder
                string sourceFilePath = openFileDialog.FileName;

                try
                {
                    // local path to resources of project
                    string resourcesDirectory = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources");

                    // if it doesn't exist then it gets created
                    if (!Directory.Exists(resourcesDirectory))
                    {
                        Directory.CreateDirectory(resourcesDirectory);
                    }

                    // name of folder from absolute path
                    string fileName = Path.GetFileName(sourceFilePath);

                    // new path to local path
                    string destinationFilePath = Path.Combine(resourcesDirectory, fileName);

                    // copy image to local path
                    File.Copy(sourceFilePath, destinationFilePath, true);

                    string relativePath = Path.Combine("Resources", fileName);
                    imagePathTextBox.Text = relativePath;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Eroare la selectarea imaginii: " + ex.Message);
                }
            }
        }
        void addWord()
        {
            // category empty
            if (string.IsNullOrEmpty(categoryComboBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți o categorie.");
                return;
            }

            // word empty
            if (string.IsNullOrEmpty(wordTextBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți un cuvânt.");
                return;
            }

            // description empty
            if (string.IsNullOrEmpty(descriptionTextBox.Text))
            {
                MessageBox.Show("Vă rugăm să introduceți o descriere.");
                return;
            }
            // Crate word object
            Word word = new Word
            {
                Category = categoryComboBox.Text,
                WordText = wordTextBox.Text,
                Description = descriptionTextBox.Text,
                ImagePath = imagePathTextBox.Text
            };

            if (string.IsNullOrEmpty(word.Category))
            {
                MessageBox.Show("Vă rugăm să introduceți o categorie.");
                return;
            }

            // read from xml if it exists
            List<Word> existingWords = new List<Word>();
            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    existingWords = (List<Word>)serializer.Deserialize(fileStream);
                }
            }

            // verify if word already exists
            bool wordExists = existingWords.Any(w => w.WordText.Equals(word.WordText, StringComparison.OrdinalIgnoreCase));
            if (wordExists)
            {
                MessageBox.Show("Cuvântul este deja adăugat.");
                return;
            }

            // add word and serialize in xml
            existingWords.Add(word);
            XmlSerializer serializer2 = new XmlSerializer(typeof(List<Word>));
            using (FileStream fileStream = new FileStream("words.xml", FileMode.Create))
            {
                serializer2.Serialize(fileStream, existingWords);
            }

            MessageBox.Show("Cuvântul a fost adăugat cu succes!");
        }
        
        
    }

    public class Word
    {
        [XmlElement("Category")]
        public string Category { get; set; }

        [XmlElement("Word")]
        public string WordText { get; set; }

        [XmlElement("Description")]
        public string Description { get; set; }

        [XmlElement("ImagePath")]
        public string ImagePath { get; set; }
    }
}