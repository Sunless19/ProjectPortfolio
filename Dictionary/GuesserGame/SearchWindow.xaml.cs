using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using System.Xml.Serialization;

namespace GuesserGame
{
    public partial class SearchWindow : Window
    {
        public SearchWindow()
        {
            InitializeComponent();
        }
        
        private void SearchButton_Click(object sender, RoutedEventArgs e)
        {
            search();
        }
        private void searchTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            searchTextBoxCH();
        }
        private void ResetButton_Click(object sender, RoutedEventArgs e)
        {
            // Close current open window.
            this.Close();

            // Open same window.
            SearchWindow newWindow = new SearchWindow();
            newWindow.Show();
        }
        private void SearchWindow_Loaded(object sender, RoutedEventArgs e)
        {
            // Call the method to populate the ComboBox with categories
            PopulateCategoryComboBox();
        }
        private List<Word> SearchWords(string searchText, string selectedCategory)
        {
            List<Word> results = new List<Word>();

            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    List<Word> words = (List<Word>)serializer.Deserialize(fileStream);

                    // Filter words based on search text and selected category
                    results = words.Where(word =>
    (string.IsNullOrEmpty(selectedCategory) || word.Category.Equals(selectedCategory)) &&
    word.WordText.Contains(searchText)).Select(word => new Word
    {
        Category = word.Category, 
        WordText = word.WordText, 
        Description = word.Description, 
        ImagePath = word.ImagePath 
    }).ToList();
                }
            }

            return results;
        }
        private void PopulateCategoryComboBox()
        {
            if (File.Exists("words.xml") && new FileInfo("words.xml").Length > 0)
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Word>));
                using (FileStream fileStream = new FileStream("words.xml", FileMode.Open))
                {
                    List<Word> words = (List<Word>)serializer.Deserialize(fileStream);

                    // Extract unique categories
                    var categories = words.Select(word => word.Category).Distinct().ToList();

                    // Clear existing items in ComboBox
                    categoryComboBox.Items.Clear();

                    // Add categories to ComboBox
                    foreach (var category in categories)
                    {
                        categoryComboBox.Items.Add(category);
                    }
                }
            }
        }
        void search()
        {
            string searchText = searchTextBox.Text;
            string selectedCategory = categoryComboBox.SelectedItem as string;

            List<Word> searchResults = SearchWords(searchText, selectedCategory);

            // Clear existing results
            searchResultsListBox.Items.Clear();

            if (searchResults.Count == 1)
            {
                DisplaySingleWordDetails(searchResults[0]);
            }
            else
            {
                // Dacă există mai mult de un rezultat, afișează în ListBox
                foreach (Word result in searchResults)
                {
                    searchResultsListBox.Items.Add(result);
                }
            }
        }
        void searchTextBoxCH()
        {
            string searchText = searchTextBox.Text;

            if (string.IsNullOrWhiteSpace(searchText))
            {
                searchResultsListBox.Items.Clear();
                return;
            }

            string selectedCategory = categoryComboBox.SelectedItem as string;

            List<Word> searchResults = SearchWords(searchText, selectedCategory);

            // Clear existing results
            searchResultsListBox.Items.Clear();

            // Add search results to ListBox
            foreach (Word result in searchResults)
            {
                searchResultsListBox.Items.Add(result);
            }
        }
        private void DisplaySingleWordDetails(Word word)
        {
            // details for a single word.
            singleResultDetails.Visibility = Visibility.Visible;
            singleResultTextBlock.Text = $"Word: {word.WordText}\nCategory: {word.Category}\nDescription: {word.Description}";

            if (!string.IsNullOrEmpty(word.ImagePath))
            {
                searchResultsListBox.Visibility = Visibility.Collapsed;
                // Absolute path to image
                string imagePath = Path.Combine(Environment.CurrentDirectory, word.ImagePath);

                // Verify file path exists.
                if (File.Exists(imagePath))
                {
                    try
                    {
                        // BitMapImage
                        BitmapImage bitmap = new BitmapImage();
                        bitmap.BeginInit();
                        bitmap.UriSource = new Uri(imagePath);
                        bitmap.EndInit();

                        // new image + setting source.
                        Image image = new Image();
                        image.Source = bitmap;

                        // add image to unique panel
                        singleResultDetails.Children.Add(image);
                    }
                    catch (Exception ex)
                    {
                        // Throw error if loading image
                        MessageBox.Show($"Error loading image: {ex.Message}", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }
                }
                else
                {
                    // Throw error if file doesn't exist
                    MessageBox.Show("Image file not found.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
        }
    }
}