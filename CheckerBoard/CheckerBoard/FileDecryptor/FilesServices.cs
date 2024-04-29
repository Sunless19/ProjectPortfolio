using CheckerBoard.Models;
using Microsoft.Win32;
using System.IO;

namespace CheckerBoard.Services
{
    public class FilesService
    {
        private const string WinsFileName = "wins.json";

        public void SaveWins(PlayerWins wins)
        {
            var jsonString = JsonService.Serialize(wins);
            File.WriteAllText(WinsFileName, jsonString);
        }
        public PlayerWins LoadWins()
        {
            if (File.Exists(WinsFileName))
            {
                var jsonString = File.ReadAllText(WinsFileName);
                return JsonService.Deserialize<PlayerWins>(jsonString);
            }

            return new PlayerWins();
        }
        public void SaveObjectToFile<T>(T objectToSerialize)
        {
            var saveFileDialog = new SaveFileDialog
            {
                Filter = "JSON files (*.json)|*.json|All files (*.*)|*.*",
                Title = "Save as JSON"
            };

            if (saveFileDialog.ShowDialog() == true)
            {
                string jsonString = JsonService.Serialize(objectToSerialize);
                File.WriteAllText(saveFileDialog.FileName, jsonString);
            }
        }

        public T LoadObjectFromFile<T>()
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "JSON files (*.json)|*.json|All files (*.*)|*.*",
                Title = "Open JSON File"
            };

            if (openFileDialog.ShowDialog() == true)
            {
                string jsonString = File.ReadAllText(openFileDialog.FileName);
                return JsonService.Deserialize<T>(jsonString);
            }

            return default;
        }

    }
}
