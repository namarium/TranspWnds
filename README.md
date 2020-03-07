# TranspWnds <span style="font-size: 70%">1.4.1</span> [![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
To build "TranspWnds" https://code.google.com/archive/p/transpwnds/

## このリポジトリについて
オープンソースのフリーソフト「TranspWnds」をビルドするためのリポジトリです.  

下記の都合で、公開されているソースがそのままでは扱いづらいので整備します.
- 文字コードが「Cyrillic(Windows 1251)」
  - 日本語環境のVisual Studioでは,「Shift_JIS」と判定されてしまいます.
  - → 「UTF-8」に変換します.
- コアライブラリ「ULLib」が別管理
  - VS2008用のソリューションが提供されていますが,依存関係のあるライブラリが同梱されていないので,別途設定が必要です.
  - → 同梱します.

## ビルド環境
- Visual Studio 2015 
- x86(Win32) 構成のみ
  - x64 向けにビルドするには、「ULLib」を含めて修正が必要になります.

## バージョン
※ ビルドのために一部修正しています.
- transpwnds: 1.4.1
- ullib: 2.3.7

## ライセンス
「TranspWnds」「ULLib」共に「LGPL-3.0」です.  
各フォルダの「COPYING.LESSER」が、配布元のデータです.

# リンク
- transpwnds: https://code.google.com/archive/p/transpwnds/
- ullib: https://code.google.com/archive/p/ullib/
