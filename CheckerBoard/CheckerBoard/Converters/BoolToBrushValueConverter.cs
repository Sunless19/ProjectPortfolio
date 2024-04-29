using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace CheckerBoard.Converters
{
    class BoolToBrushValueConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool && (bool)value)
            {
                return Brushes.DarkGreen;
            }
            return Brushes.Wheat;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
