using System;
using System.Collections.Generic;
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
using System.IO;
using System.Xml.Serialization;

namespace GuesserGame
{
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        private List<Word> words = new List<Word>();
        public LoginWindow()
        {
            InitializeComponent();
            addWord.Visibility = Visibility.Collapsed;
            modifyWord.Visibility = Visibility.Collapsed;
            deleteWord.Visibility = Visibility.Collapsed;
        }

        private void usernameTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
        private void AddWordButton_Click(object sender, RoutedEventArgs e)
        {
            AddWordWindow addWordWindow = new AddWordWindow();
            addWordWindow.ShowDialog();
        }
        private void DeleteWordButton_Click(object sender, RoutedEventArgs e)
        {
            DeleteClick();
        }
        private void ModifyWordButton_Click(object sender, RoutedEventArgs e)
        {
            ModifyClick();
        }
        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            LoginClick();
        }

        void DeleteClick()
        {
            DeleteWordDialog deleteWordDialog = new DeleteWordDialog();
            if (deleteWordDialog.ShowDialog() == true)
            {
                string wordToDelete = deleteWordDialog.WordToDelete;
                if (string.IsNullOrWhiteSpace(wordToDelete))
                {
                    MessageBox.Show("Vă rugăm să introduceți un cuvânt pentru ștergere.");
                    return;
                }
                //Read from .xml if it exists
                List<Word> existingWords = new List<Word>();
                if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                    using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                    {
                        existingWords = (List<Word>)serializer.Deserialize(fileStream);
                    }
                }

                //Find word and remove it from list
                Word wordToRemove = existingWords.FirstOrDefault(w => w.WordText == wordToDelete);
                if (wordToRemove != null)
                {
                    existingWords.Remove(wordToRemove);
                    //Serialization of list in .xml after the list has been refreshed
                    XmlSerializer serializer2 = new XmlSerializer(typeof(List<Word>));
                    using (FileStream fileStream = new FileStream("words.xml", FileMode.Create))
                    {
                        serializer2.Serialize(fileStream, existingWords);
                    }

                    MessageBox.Show("Cuvântul a fost șters cu succes!");
                }
                else
                {
                    MessageBox.Show("Cuvântul nu a fost găsit în lista existentă.");
                }
            }
        }
        void ModifyClick()
        {
            ModifyWordDialog modifyDialog = new ModifyWordDialog();
            if (modifyDialog.ShowDialog() == true)
            {
                //Verify if a new word has been added after modification
                if (string.IsNullOrEmpty(modifyDialog.WordToModify))
                {
                    MessageBox.Show("Vă rugăm să introduceți un cuvânt pentru modificare.");
                    return;
                }

                //Read from .xml if it exists
                List<Word> existingWords = new List<Word>();
                if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                    using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                    {
                        existingWords = (List<Word>)serializer.Deserialize(fileStream);
                    }
                }

                //Search for word to modify in existent list
                Word wordToModify = existingWords.FirstOrDefault(w => w.WordText.Equals(modifyDialog.WordToModify, StringComparison.OrdinalIgnoreCase));
                if (wordToModify == null)
                {
                    MessageBox.Show("Cuvântul specificat nu a fost găsit în lista cuvintelor existente.");
                    return;
                }

                //Refresh members to new input values
                wordToModify.Category = modifyDialog.NewCategory;
                wordToModify.Description = modifyDialog.NewDescription;
                wordToModify.ImagePath = modifyDialog.NewImagePath;

                //Serialization of list in .xml
                XmlSerializer serializer2 = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Create))
                {
                    serializer2.Serialize(fileStream, existingWords);
                }

                MessageBox.Show("Cuvântul a fost modificat cu succes!");
            }
        }
        void LoginClick()
        {
            string username = usernameTextBox.Text;
            string password = passwordBox.Password;


            //file
            string[] accounts = File.ReadAllLines("accounts.txt");

            bool loggedIn = false;
            foreach (string account in accounts)
            {
                string[] parts = account.Split(':');
                if (parts.Length == 2)
                {
                    string storedUsername = parts[0];
                    string storedPassword = parts[1];

                    if (username == storedUsername && password == storedPassword)
                    {
                        loggedIn = true;
                        break;
                    }
                }
            }

            // Login
            if (loggedIn)
            {
                //mainGrid.Visibility = Visibility.Collapsed; All grid collapses. Nothing visible
                addWord.Visibility = Visibility.Visible;
                modifyWord.Visibility = Visibility.Visible;
                deleteWord.Visibility = Visibility.Visible;
                Password.Visibility = Visibility.Collapsed;

                Username.Visibility = Visibility.Collapsed;
                usernameTextBox.Visibility = Visibility.Collapsed;
                passwordBox.Visibility = Visibility.Collapsed;
                Login.Visibility = Visibility.Collapsed;
            }
            else
            {
                MessageBox.Show("Nume de utilizator sau parola incorecta!");
            }
        }

    }
}
