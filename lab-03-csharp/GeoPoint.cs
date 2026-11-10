namespace Atmosphere;

/// <summary>
/// Географическая точка - тип-значение (struct).
/// Изменяемая структура здесь сделана намеренно, только чтобы показать копирование при присваивании.
/// </summary>
public struct GeoPoint
{
    public double Latitude { get; set; }
    public double Longitude { get; set; }

    public override string ToString() => $"({Latitude}, {Longitude})";
}
