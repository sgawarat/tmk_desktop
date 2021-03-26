# TMK Desktop

## これは何？

あなたのキー入力によって動作する仮想的なキーボードを常駐させるデスクトップアプリケーションです。キーボードエンジンとしてキーボードファームウェアライブラリである「[TMK](https://github.com/tmk/tmk_keyboard)」のコアを利用しており、キーを入れ替えるだけではない様々なカスタマイズを行うことができます。

## ビルド

### 必要なもの

- GCCまたはClang
- CMake
- [tmk_core](https://github.com/tmk/tmk_core)
  - `TMK_CORE_DIR`ディレクトリを参照します
- キーマップ定義
  - `TMK_DESKTOP_KEYMAP_DIR`ディレクトリを参照します

## キーマップ

- TMKのactionmap定義がそのまま利用できます
- OSとTMKを接続するための変換表を定義する必要があります

## ライセンス

GPLv2 or later
