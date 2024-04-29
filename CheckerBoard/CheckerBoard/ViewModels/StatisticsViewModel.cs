using CheckerBoard.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CheckerBoard.ViewModels
{
    internal class StatisticsViewModel : INotifyPropertyChanged
    {
        private int _blackWins;
        private int _whiteWins;
        private int _maxPiecesRemaining;
        public StatisticsViewModel() { }

        public StatisticsViewModel(GameModel gameModel)
        {
            // Setarea valorilor initiale
            BlackWins = gameModel.BlackWins;
            WhiteWins = gameModel.WhiteWins;
            MaxPiecesRemaining = gameModel.MaxPiecesRemaining;
        }
        public int BlackWins
        {
            get => _blackWins;
            set
            {
                if (_blackWins != value)
                {
                    _blackWins = value;
                    OnPropertyChanged(nameof(BlackWins));
                }
            }
        }

        public int WhiteWins
        {
            get => _whiteWins;
            set
            {
                if (_whiteWins != value)
                {
                    _whiteWins = value;
                    OnPropertyChanged(nameof(WhiteWins));
                }
            }
        }

        public int MaxPiecesRemaining
        {
            get => _maxPiecesRemaining;
            set
            {
                if (_maxPiecesRemaining != value)
                {
                    _maxPiecesRemaining = value;
                    OnPropertyChanged(nameof(MaxPiecesRemaining));
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
