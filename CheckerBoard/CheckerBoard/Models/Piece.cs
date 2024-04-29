using CheckerBoard.ViewModels;

namespace CheckerBoard.Models
{
    public class Cell : BaseViewModel
    {
        private bool _isBlack;
        private bool _isOccupied;
        private CheckerTypes _content;
        private bool _isSelected;
        public int RowIndex { get; set; }
        public int ColumnIndex { get; set; }
        public bool IsBlack
        {
            get { return _isBlack; }
            set
            {
                _isBlack = value;
                OnPropertyChanged(nameof(IsBlack));
            }
        }

        public bool IsOccupied
        {
            get { return _isOccupied; }
            set
            {
                _isOccupied = value;
                OnPropertyChanged(nameof(IsOccupied));
            }
        }

        public CheckerTypes Content
        {
            get { return _content; }
            set
            {
                _content = value;
                OnPropertyChanged(nameof(Content));
            }
        }

        public bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                _isSelected = value;
                OnPropertyChanged(nameof(IsSelected));
            }
        }

        public Cell() { }
        public Cell(bool isBlack, CheckerTypes content = default, int rowIndex = 0, int columnIndex = 0)
        {
            IsBlack = isBlack;
            if (content != default)
            {
                IsOccupied = true;
            }
            else
            {
                IsOccupied = false;
            }
            Content = content;

            // Inițializăm proprietățile de index
            RowIndex = rowIndex;
            ColumnIndex = columnIndex;
        }
        
    }

}

