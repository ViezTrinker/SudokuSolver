#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

inline constexpr int32_t ScreenWidth = 600;
inline constexpr int32_t ScreenHeight = 445;

inline constexpr int32_t PixelWidth = 1;
inline constexpr int32_t PixelHeight = 1;

inline constexpr int32_t TopLayerHeight = 40;
inline constexpr int32_t TopLayerWidth = ScreenWidth;

inline constexpr int32_t BoardWidth = 405;
inline constexpr int32_t BoardHeight = ScreenHeight - TopLayerHeight;

inline constexpr int32_t InfoBoxWidth = ScreenWidth - BoardWidth;
inline constexpr int32_t InfoBoxHeight = BoardHeight;

inline constexpr int32_t SudokuUnits = 9;
inline constexpr int32_t BlockUnits = 3;

inline constexpr int32_t SudokuUnitWidth = BoardWidth / SudokuUnits;
inline constexpr int32_t SudokuUnitHeight = BoardHeight / SudokuUnits;

static_assert(BoardWidth == BoardHeight);
static_assert(BoardWidth % SudokuUnits == 0);

#endif