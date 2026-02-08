type Geometry = "circular" | "rectangular";

interface Parameters {
    GRID_COLUMNS: number;
    GRID_ROWS: number;
    LED_BASE_BRIGHTNESS: number;
    LED_BASE_TONE: number;
    LED_GEOMETRY: Geometry;
    LED_SIZE_HORIZONTAL: number;
    LED_SIZE_VERTICAL: number;
    MANUFACTURER: string;
    MODEL: string;
    PITCH_HORIZONTAL: number;
    PITCH_VERTICAL: number;
}

const IkeaFrekvensParameters: Parameters = {
    GRID_COLUMNS: 16, // px
    GRID_ROWS: 16, // px
    LED_BASE_BRIGHTNESS: 2 ** 6,
    LED_BASE_TONE: 2 ** 4,
    LED_GEOMETRY: "circular",
    LED_SIZE_HORIZONTAL: 4, // mm
    LED_SIZE_VERTICAL: 4, // mm
    MANUFACTURER: "IKEA",
    MODEL: "Frekvens",
    PITCH_HORIZONTAL: 6, // mm
    PITCH_VERTICAL: 6, // mm
};

const IkeaObegransadParameters: Parameters = {
    GRID_COLUMNS: 16, // px
    GRID_ROWS: 16, // px
    LED_BASE_BRIGHTNESS: 2 ** 5,
    LED_BASE_TONE: 0,
    LED_GEOMETRY: "rectangular",
    LED_SIZE_HORIZONTAL: 12.5, // mm
    LED_SIZE_VERTICAL: 12.5, // mm
    MANUFACTURER: "IKEA",
    MODEL: "Obegränsad",
    PITCH_HORIZONTAL: 20, // mm
    PITCH_VERTICAL: 30, // mm
};

export const Device: Parameters =
    import.meta.env.VITE_IKEA_FREKVENS === "true"
        ? IkeaFrekvensParameters
        : import.meta.env.VITE_IKEA_OBEGRANSAD === "true"
          ? IkeaObegransadParameters
          : ({} as Parameters);

export const IkeaFrekvens: string = `${IkeaFrekvensParameters.MANUFACTURER} ${IkeaFrekvensParameters.MODEL}`;
export const IkeaObegransad: string = `${IkeaObegransadParameters.MANUFACTURER} ${IkeaObegransadParameters.MODEL}`;
