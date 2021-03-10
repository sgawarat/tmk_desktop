# TMK Desktop

## これは何？

あなたのキー入力によって動作する仮想的なキーボードを常駐させるデスクトップアプリケーションです。キーボードエンジンとしてキーボードファームウェアライブラリである「[TMK](https://github.com/tmk/tmk_keyboard)」のコア部分を利用しており、キーを入れ替えるだけではない様々なカスタマイズを行うことができます。

## ビルド

### 必要なもの

- GCCまたはClang
- CMake

### 依存するライブラリ

- [tmk_core](https://github.com/tmk/tmk_core)
  - キーボードエンジン
  - GPLv2 or later
- [fmt](https://github.com/fmtlib/fmt)
  - 文字列整形
  - MIT License

## ライセンス

GPLv2 or later
