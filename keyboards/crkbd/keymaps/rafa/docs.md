# Rafa's Corne Layout

## Notation

`tapShift|hold400ms` — `|` separates tap (and Shift+tap) from the 400 ms hold action.
`^X` = Ctrl+X.  `S-` = Shift+.  `Pipe` = literal `|` (used as separator above, so spelled out).

---

## Layer 0 — Base

```
   `~|Fn   qQ|!    wW|@   eE|#     rR|$  tT|%      |  yY|^    uU|&  iI|*   oO|(   pP|)   \Pipe|Fn
Esc|Ctrl   aA|1    sS|2   dD|3     fF|4  gG|5      |  hH|6    jJ|7  kK|8   lL|9   ;:|0   '"|Win
   Shift   zZ|^Z   xX|_   cC|-     vV|+  bB|=      |  nN|}    mM|{  ,<|[   .>|]   /?|^/  Shift
                 Tab|Ctrl  Space  Enter|Alt          |  Del|AltGr  Bksp  S-Tab|Ctrl
```

## Layer 1 — Fn (hold ` or \)

```
_______    F11     F12    ---      ---   ---       |  CapsLk  Ins   NumLk  Pause  ScLk   _______
Esc|Ctrl   F1      F2     F3       F4    F5        |  F6      F7    F8     F9     F10    Win
   Shift   ---     Play   Stop     Prev  Next      |  Mute    Vol-  Vol+   Bri-   Bri+   Shift
                 Tab|Ctrl  Space  Enter|Alt          |  Del|AltGr  Bksp  S-Tab|Ctrl
```

---

## Combos

All combos are **vertical** — same column, two rows pressed simultaneously.

### Arrows — right hand, home + top row

| Combo | Action |
|-------|--------|
| H + Y | ←      |
| J + U | ↓      |
| K + I | ↑      |
| L + O | →      |

### Extended navigation — right hand, home + bottom row

| Combo | Action              |
|-------|---------------------|
| H + N | Ctrl+← (prev word)  |
| J + M | Shift+↓             |
| K + , | Shift+↑             |
| L + . | Ctrl+→ (next word)  |

### Page navigation — left hand, home + top row

| Combo | Action |
|-------|--------|
| S + W | Home   |
| D + E | PgUp   |
| F + R | PgDn   |
| G + T | End    |

### Extended page — left hand, home + bottom row

| Combo | Action        |
|-------|---------------|
| S + X | Shift+Home    |
| D + C | Ctrl+PgUp     |
| F + V | Ctrl+PgDn     |
| G + B | Shift+End     |

### Misc

| Combo | Action      |
|-------|-------------|
| P + ' | PrintScreen |

---

## Thumb cluster

```
Left:   Tab|Ctrl   Space   Enter|Alt
Right:  Del|AltGr  Bksp    S-Tab|Ctrl
```

- **S-Tab|Ctrl**: tap = Shift+Tab, hold = Ctrl (activates immediately when
  another key is pressed while this thumb is held ≥ 50 ms).
- **Del|AltGr**: AltGr hold for Portuguese accents (ã, ç, etc.).

---

## Hold behaviour notes

- 400 ms threshold globally for all tap-hold keys.
- **Tap-tap-hold repeats the tap** keycode (e.g. quickly tap Q then hold → `qqq…`, not `!!!`).
- **Z hold** = Ctrl+Z — undo in GUI apps; SIGTSTP (suspend process) in bash.
- **/ hold** = Ctrl+/ — toggle line comment in VSCode / most editors.
- **Play** = KC_MPLY (play/pause toggle); **Stop** = KC_MSTP.
