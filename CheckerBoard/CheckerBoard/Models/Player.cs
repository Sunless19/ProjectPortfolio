using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CheckerBoard.Models
{
    public enum Player
    {
        None,
        Black,
        White
    }
    public class PlayerWins
    {
        public int BlackWins { get; set; }
        public int WhiteWins { get; set; }
    }
}
