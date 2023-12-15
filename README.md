# tuim
An immediate mode text-based user interface for terminals.

## Containers

```
<-------- width -------->  
------------------------- ^  
| [X] button            | |  
|                       | height  
|                       | |  
|                       | |  
------------------------- v  
```

The size of the container is adjusted when printing elements. If the container size is fixed, and
the size of the elements is greater than the container's, a scrollbar will appear.


### Columns

Columns are containers printed on the same line as the other columns in a parent container.
The column go to the next line when there isn't enough space horizontally in the container.

```
---------------------------------
| -------------- -------------- |
| |            | |            | |
| |  column 1  | |  column 2  | |
| |            | |            | |
| |            | |            | |
| -------------- -------------- |
| --------------                |
| |            |                |
| |  column 3  |                |
| |            |                |
| |            |                |
| --------------                |
---------------------------------
```

## Components

### Text

```cpp
tuim::print(fmt, ...);
```

```cpp
tuim::text(id, str);
```
<small><i>Note: use `tuim::print` if no id needed.</i></small>

### Paragraph

```cpp
tuim::paragraph(id, text, width);
```
![paragraph](https://cdn.discordapp.com/attachments/1137766959203745863/1185100857101004831/paragraph.gif)

### Buttons

```cpp
bool is_pressed = tuim::button(id, text, flags);
```
![buttons](https://cdn.discordapp.com/attachments/1137766959203745863/1185101731810521149/buttons.gif)

### Sliders

```cpp
bool has_changed = tuim::slider<int>(id, &value, min, max, step);
```
![int slider](https://cdn.discordapp.com/attachments/1137766959203745863/1185101001406042152/int-slider.gif)
![float slider](https://cdn.discordapp.com/attachments/1137766959203745863/1185100419819647016/float-slider.gif)

### Inputs

#### Numbers
```cpp
bool has_changed = tuim::input_number<int>(id, "my value: <%d>", &value, min, max, step);
```
![int input](https://cdn.discordapp.com/attachments/1137766959203745863/1185103040613720124/int-input.gif)

```cpp
bool has_changed = tuim::input_enum<my_enum>(id, "my value: < %s >", &value, max, labels);
```
![enum input](https://cdn.discordapp.com/attachments/1137766959203745863/1185101123955195924/enum-input.gif)

```cpp
bool has_changed = tuim::input_bool(id, "my boolean: %s", &value, [optional] labels);
```

#### Texts
```cpp
bool has_changed = tuim::input_text(id, &value, default_value, flags);
```
![text input](https://cdn.discordapp.com/attachments/1137766959203745863/1185100683821723708/text-input.gif)

Available flags:
- INPUT_TEXT_FLAGS_NONE
- INPUT_TEXT_FLAGS_NUMERIC_ONLY
- INPUT_TEXT_FLAGS_ALPHANUMERIC_ONLY
- INPUT_TEXT_ALLOW_LINE_BREAKS

### Misc

```cpp
tuim::hr(length);
```

## Cursor

### Position

The cursor position is stored globally. When `tuim::print(fmt, ...)` is called, it checks the real length of `fmt` and move the cursor accordingly.
It also updates the container size and position.

```cpp
tuim::vec2 pos = tuim::get_cursor();
```

```cpp
tuim::gotoxy(pos);
// OR
tuim::gotoxy({x, y});
```

### Style

```cpp
tuim::set_cursor_visible(false);
```

## Inline Style Tags

### Inline Colors

Use `#` and `#_` to change the foreground and background color respectively:

```cpp
tuim::print("#ff0000Foreground");
tuim::print("#_ff0000Background");
```

You can add a semicolon after the color code for readability:

```cpp
tuim::print("#ff0000;Foreground");
```

### Text Modes

- `&r` clear colors stack and reset color to default.  
- `&b` toggle bold style.
- `&d` toggle dim style.
- `&i` toggle italic style.
- `&u` toggle underlined style.
- `&g` toggle blinking style.
- `&n` toggle inverse style.
- `&h` toggle hidden style.
- `&s` toggle strikethrough style.

### Custom Codes

```cpp
font::register_style("a", font::make_style(color::from_code("#ff0000")));
font::register_style("k", font::make_style(font::mode::BOLD));

tuim::print("&aGreen Foreground");
tuim::print("&_aGreen Background");
tuim::print("&kBold");
```