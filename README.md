# TMK Desktop

## これは何？

あなたのキー入力によって動作する仮想的なキーボードを常駐させるデスクトップアプリケーションです。キーボードエンジンとしてキーボードファームウェアライブラリである「[TMK](https://github.com/tmk/tmk_keyboard)」のコアを利用しており、キーを入れ替えるだけではない様々なカスタマイズを行うことができます。

## ビルド

### 必要なもの

- C++20をビルドできる環境（GCCまたはClang）
- CMake（3.18以上）
- [tmk_core](https://github.com/tmk/tmk_core)
  - `TMK_CORE_DIR`ディレクトリを参照します。
- キーマップ
  - `TMK_DESKTOP_KEYMAP_DIR`ディレクトリを参照します。

## キーマップ

キーマップは仮想キーボードの大きさや挙動を定義するものです。TMK Desktopでは、`TMK_DESKTOP_KEYMAP_DIR`で指定されるディレクトリにてビルドされる`keyboard`というライブラリをキーマップとしてリンクします。また、同ディレクトリ内に`config.h`というヘッダーファイルを必要とし、TMKを含むすべてのソースファイルにインクルードします。詳細は`keyboards`ディレクトリ内の作例を参照してください。

### コンフィグ

`config.h`では以下を定義します。

- `MATRIX_COLS`マクロ、`MATRIX_ROWS`マクロ
  - 仮想キーボードの大きさを二次元で指定します。
  - `MATRIX_COLS`は1から32までを、`MATRIX_ROWS`は1から255までを指定できます。
<!--
- 任意：`TMK_DESKTOP_NOIMPL_KEYCODE_TO_SCANCODE_TABLE`マクロ
  - このマクロを定義すると、キーコードからスキャンコードへの変換表を既定で定義しません。
  - その変換表は`src/win32/scancode.cpp`にあります。
-->

### アクションマップ

`keyboard`ライブラリでは、TMKの仕様に従って`actionmaps`をそのまま定義できます。また、`action_for_key`、`action_get_macro`、`action_function`などの関数もそのまま定義でき、TMKの各種関数を呼び出すこともできます。

### 物理キーボードと仮想キーボードの接続

`keyboard`ライブラリでは、物理キーボードと仮想キーボードをつなぐための対応表を以下のように定義する必要があります。これらは`include/tmk_desktop/<platform名>/settings.hpp`をインクルードした上で、名前空間`tmk_desktop::inline <platform名>`内に定義します。

- `key_to_keypos_table`
  - `KeyToKeyposTable`型の定数であり、物理キーボードのキーから仮想キーボードのキーへの対応を定義する配列です。
  - `Key`は物理キーボードのキーを表す連続的な値であり、`include/tmk_desktop/<platform名>/event.hpp`で定義されます。
  - `keypos_t`は仮想キーボードのキーを表す二次元の値であり、TMKで定義されます。

### 特殊な挙動への対処

`keyboard`ライブラリでは、OSにより発生する特殊な挙動への回避策に関する設定を以下のように定義する必要があります。これらは`include/tmk_desktop/<platform名>/settings.hpp`をインクルードした上で、名前空間`tmk_desktop::inline <platform名>`内に定義します。

- `tapping_key_table`
  - `TappingKeyTable`型の定数であり、押したと同時に離したと解釈するキーを指定する配列です。
  - 物理キーボードのキーを表す`Key`を添字として、設定を有効化するかどうかの`bool`を格納します。
  - 離したことをOSが教えてくれないキーに対して、押したままになる不具合を回避するために使用します。
    - 日本語配列では「半角全角」「Caps Lock」「カタカナひらがな」などが該当します。
  - この設定を有効化したキーは長押しによる挙動が機能しなくなります。
    - ただし、キーリピートは通常のように発生します。

## ライセンス

GPLv2 or later
