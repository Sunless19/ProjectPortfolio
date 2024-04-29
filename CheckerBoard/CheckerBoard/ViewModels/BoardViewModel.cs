using System.Windows;
using System.Windows.Input;
using CheckerBoard;
using CheckerBoard.Services;
using CheckerBoard.Models;
using CheckerBoard.Views;
using System.Windows.Controls;

namespace CheckerBoard.ViewModels
{
    public class BoardViewModel : BaseViewModel
    {
        private readonly FilesService _jsonService;

        private GameModel _gameModel;
        public ICommand ClickCellCommand { get; set; }

        public ICommand MovePieceCommand { get; set; }

        public ICommand SaveGameCommand { get; set; }

        public ICommand LoadGameCommand { get; set; }

        public ICommand NewGameCommand { get; set; }

        public ICommand MultipleJumpCommand { get; set; }

        public ICommand DisplayInfoCommand { get; set; }

        public ICommand DisplayStatisticsCommand { get; set; }

        public GameModel GameModel
        {
            get => _gameModel;
            set
            {
                if (_gameModel != value)
                {
                    _gameModel = value;
                    OnPropertyChanged(nameof(GameModel)); 
                    OnPropertyChanged(nameof(GameModel.Cells)); 
                }
            }
        }

        public void NewGame(object parameter)
        {
            MessageBoxResult result = MessageBox.Show(
            "Will the new game support multiple jumps?",
            "New Game Settings",
            MessageBoxButton.YesNo,
            MessageBoxImage.Question);

            if (result == MessageBoxResult.Yes)
            {
               GameModel = new GameModel(true);
               GameModel.HasMultipleJumps = true;
            }
            else if (result == MessageBoxResult.No)
            {
                GameModel  = new GameModel(true);
                GameModel.HasMultipleJumps = false;
            }
        }
        public void SaveGame(object parameter)
        {
            _jsonService.SaveObjectToFile(GameModel);
        }
        public void LoadGame(object parameter)
        {
            var loadedGameModel = _jsonService.LoadObjectFromFile<GameModel>();
            if (loadedGameModel != null)
            {
                GameModel.Cells.Clear(); // Elimină celulele existente pentru a evita dublarea acestora
                foreach (var cell in loadedGameModel.Cells)
                {
                    GameModel.Cells.Add(cell); // Adaugă celulele încărcate în modelul actual
                    
                }
                GameModel.CurrentPlayer = loadedGameModel.CurrentPlayer;
                GameModel.IsGameNotInProgress = loadedGameModel.IsGameNotInProgress;
                GameModel.SelectedCell = loadedGameModel.SelectedCell;
                GameModel.Winner = loadedGameModel.Winner;
                GameModel.HasMultipleJumps = loadedGameModel.HasMultipleJumps;
                OnPropertyChanged(nameof(GameModel.BlackPieceCount));
                OnPropertyChanged(nameof(GameModel.WhitePieceCount));
            }
        }

        public BoardViewModel()
        {
            GameModel = new GameModel(true);
            _jsonService = new FilesService();

            NewGameCommand = new RelayCommand(NewGame);
            SaveGameCommand = new RelayCommand(SaveGame);
            LoadGameCommand = new RelayCommand(LoadGame);
            DisplayStatisticsCommand = new RelayCommand(DisplayStatistics);
            MultipleJumpCommand = new RelayCommand(MultipleJump);
            DisplayInfoCommand = new RelayCommand(DisplayInfo);
            ClickCellCommand = new RelayCommand(ExecuteClickCell);

        }

        private void ExecuteClickCell(object parameter)
        {
            
            var gameModel = GameModel;
            var cell = parameter as Cell;

            if (cell == null)
                return;

            gameModel.IsGameNotInProgress = false;

            if (cell.IsOccupied && gameModel != null && gameModel.IsMultipleCaptureInProgress == false)
            {
                if (gameModel.SelectedCell != cell)
                {
                    if (gameModel.SelectedCell != null)
                        gameModel.SelectedCell.IsSelected = false;

                    cell.IsSelected = true;
                    gameModel.SelectedCell = cell;
                }
                else
                {
                    cell.IsSelected = false;
                }
            }
            else
            {
                if (gameModel != null && gameModel.SelectedCell != null)
                {
                    var sourceCell = gameModel.SelectedCell;
                    var destinationCell = cell;

                    if (gameModel.IsMoveValidPawn(sourceCell, destinationCell) && gameModel.IsMultipleCaptureInProgress == false)
                    {
                        gameModel.MakeMove(sourceCell, destinationCell);
                        sourceCell.IsSelected = false;
                        sourceCell.IsOccupied = false;
                        gameModel.notMovable = false;
                        if (destinationCell.Content == CheckerTypes.None)
                        {
                            destinationCell.IsOccupied = false;
                        }
                        else destinationCell.IsOccupied = true;

                    }
                    else if (gameModel.isMoveValidKing(sourceCell, destinationCell) && gameModel.IsMultipleCaptureInProgress == false)
                    {
                        gameModel.MakeMove(sourceCell, destinationCell);
                        sourceCell.IsSelected = false;
                        sourceCell.IsOccupied = false;
                        gameModel.notMovable = false;
                        if (destinationCell.Content == CheckerTypes.None)
                        {
                            destinationCell.IsOccupied = false;
                        }
                        else destinationCell.IsOccupied = true;
                    }
                    else if (gameModel.existsPieceBetween(sourceCell, destinationCell, gameModel))
                    {
                        gameModel.MakeMove(sourceCell, destinationCell);
                        if (gameModel.HasMultipleJumps == true)
                        {
                            gameModel.IsMultipleCaptureInProgress = true;
                            gameModel.CurrentPlayer = gameModel.CurrentPlayer == Player.Black ? Player.White : Player.Black;
                            destinationCell.IsSelected = true;
                            gameModel.SelectedCell = destinationCell;
                            gameModel.notMovable = true;

                        }
                        sourceCell.IsSelected = false;
                        sourceCell.IsOccupied = false;

                        if (destinationCell.Content == CheckerTypes.None)
                        {
                            destinationCell.IsOccupied = false;
                        }
                        else destinationCell.IsOccupied = true;
                    }
                    else if (gameModel.HasMultipleJumps == true && gameModel.IsMultipleCaptureInProgress == true)
                    {
                        gameModel.CurrentPlayer = gameModel.CurrentPlayer == Player.Black ? Player.White : Player.Black;
                        gameModel.IsMultipleCaptureInProgress = false;
                    }
                    
                    sourceCell.IsSelected = false;
                }
            }
        }

        private void DisplayStatistics(object obj)
        {
            StatisticsViewModel statisticsViewModel = new StatisticsViewModel(GameModel);
            StatisticsView statisticsView = new StatisticsView();
            statisticsView.DataContext = statisticsViewModel;
            statisticsView.Show();
        }

        private void DisplayInfo(object obj)
        {
            About aboutWindow = new About();
            aboutWindow.Show();
        }

        private void MultipleJump(object obj)
        {
            if (GameModel.HasMultipleJumps)
            {
                GameModel.HasMultipleJumps = false;
                return;
            }
            
            if (!GameModel.HasMultipleJumps)
            {
                GameModel.HasMultipleJumps = true;
                return;
            }
        }
    }


}