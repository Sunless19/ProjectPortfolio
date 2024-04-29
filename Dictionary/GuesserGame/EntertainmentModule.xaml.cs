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
    public partial class EntertainmentModule : Window
    {
        private int buttonClicks = 0;   //turn no. x
        private int correctGuesses = 0; //correct guesses
        private List<Word> wordsList; // Words xml
        private int currentIndex = 0; // word's current index

        public EntertainmentModule()
        {
            InitializeComponent();
            LoadWords(); // load words from xml
            ShowRandomWord();
        }
        private void NextButton_Click(object sender, RoutedEventArgs e)
        {
            nextClick();
        }

        private void ShowWord()
        {
            if (wordsList != null && wordsList.Count > 4)
            {
                Word currentWord = wordsList[currentIndex];

                // Setting description
                textBlockDescription.Text = currentWord.Description;
                // verifies the path
                if (currentWord.ImagePath.Contains("unavailable.png"))
                {
                    // if path is Resources\unavailable.png then show description
                    imageControl.Visibility = Visibility.Collapsed;
                    textBlockDescription.Visibility = Visibility.Visible;
                }
                else
                {
                    // if the image was set then choose randomly between description and image
                    Random random = new Random();
                    bool displayDescription = random.Next(2) == 0; // generates 0/1 for 
                    if (displayDescription)
                    {
                        // Description
                        textBlockDescription.Visibility = Visibility.Visible;
                        imageControl.Visibility = Visibility.Collapsed;
                    }
                    else
                    {
                        // Image
                        textBlockDescription.Visibility = Visibility.Collapsed;
                        imageControl.Visibility = Visibility.Visible;

                        // show image of word
                        string absoluteImagePath = Path.GetFullPath(currentWord.ImagePath);
                        try
                        {
                            // verifies if file for image exists.
                            if (File.Exists(absoluteImagePath))
                            {
                                // source of image
                                BitmapImage bitmap = new BitmapImage();
                                bitmap.BeginInit();
                                bitmap.UriSource = new Uri(absoluteImagePath);
                                bitmap.EndInit();

                                imageControl.Source = bitmap;
                            }
                            else
                            {
                                MessageBox.Show("Image file not found.");
                            }
                        }
                        catch (Exception ex)
                        {
                            // throws error if the image doesn't load
                            MessageBox.Show($"Error loading image: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                        }
                    }
                }

               
            }
        }
        private void LoadWords()
        {
            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    wordsList = (List<Word>)serializer.Deserialize(fileStream);
                }
            }
        }
        void nextClick()
        {
            if (wordsList != null && wordsList.Count > 0)
            {
                Word currentWord = wordsList[currentIndex];
                string guessedWord = wordInputTextBox.Text.Trim().ToLower();
                if (guessedWord == currentWord.WordText.ToLower())
                {
                    MessageBox.Show("Ai introdus cuvântul corect!");
                    correctGuesses++;

                    // removes word from word list
                    wordsList.RemoveAt(currentIndex);
                }
                else
                {
                    MessageBox.Show($"Gresit. Raspunsul corect era {currentWord.WordText}");
                }

                if (buttonClicks == 3)
                {
                    nextButton.Visibility = Visibility.Visible;
                    nextButton.Content = "Finish";
                }
                else
                {
                    nextButton.Content = "Next";
                }

                ShowRandomWord();
                buttonClicks++;

                if (buttonClicks == 5)
                {
                    MessageBox.Show($"Ai ghicit {correctGuesses} cuvinte din cele 5.");
                    EntertainmentModule entertainmentModule = new EntertainmentModule();
                    entertainmentModule.Show();
                    this.Close();
                }
            }
        }
        private void ShowRandomWord()
        {
            if (wordsList != null && wordsList.Count > 0)
            {
                Random random = new Random();
                currentIndex = random.Next(0, wordsList.Count);
                ShowWord();
            }
        }
    }
}