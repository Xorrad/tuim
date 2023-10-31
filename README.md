# tuim
A immediate mode text-based user interface for terminals.

## Cursor

### Position

The cursor position is stored globally. It is accessible using:
```cpp
tuim::vec2 pos = tuim::get_cursor();
```

When `tuim::print(fmt, ...)` is called, it checks the real length of `fmt` and move the cursor accordingly.
It also updates the container size and position.

### Style

The blinking cursor can be hidden with:
```cpp
tuim::set_cursor_visible(false);
```

## Containers



## Colors

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

### Special Codes

`&r`: clear colors stack and reset color to default.  
`&p`: pop the last style.

`&b`: toggle bold style.  
`&i`: toggle italic style.  
`&u`: toggle underlined style.  
`&s`: toggle strikethrough style.  

### Custom Codes

```cpp
font::register_style("a", font::make_style(color::from_code("#ff0000")));
font::register_style("k", font::make_style(font::mode::BOLD));

tuim::print("&aGreen Foreground");
tuim::print("&_aGreen Background");
tuim::print("&kBold");
```