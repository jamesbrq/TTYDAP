import json
import re

# Raw simple recipe table data - FILL THIS IN
SIMPLE_RECIPE_DATA = """
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_あつあつスープ_gor_00017a50
.4byte str_あつあつスープ_gor_00017a50
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコいため_gor_00017a70
.4byte str_キノコいため_gor_00017a70
.4byte str_キノコ_gor_00017a80
.4byte str_キノコいため_gor_00017a70
.4byte str_キノコいため_gor_00017a70
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコいため_gor_00017a70
.4byte str_キノコいため_gor_00017a70
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_しなびたキノコ_gor_00017ae0
.4byte zero_gor_00017170
.4byte str_キノコいため_gor_00017a70
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ドライフラワー_gor_00017b5c
.4byte zero_gor_00017170
.4byte str_あつあつスープ_gor_00017a50
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_めだまやき_gor_00017b7c
.4byte str_めだまやき_gor_00017b7c
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_ナンシーティー_gor_00017b98
.4byte str_ナンシーティー_gor_00017b98
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_生パスタ_gor_00017bbc
.4byte str_スパゲティ_gor_00017bc8
.4byte str_スパゲティ_gor_00017bc8
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ムースケーキ_gor_00017be4
.4byte str_ムースケーキ_gor_00017be4
.4byte str_レッド・カラリン_gor_00017bf4
.4byte zero_gor_00017170
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_カメカメティー_gor_00017c2c
.4byte str_カメカメティー_gor_00017c2c
.4byte str_つくしんぼ_gor_00017c3c
.4byte zero_gor_00017170
.4byte str_あつあつスープ_gor_00017a50
.4byte str_ピチピーチ_gor_00017c48
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ヤシの実_gor_00017c54
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_ゆきうさぎ_gor_00017c60
.4byte zero_gor_00017170
.4byte str_あつあつスープ_gor_00017a50
.4byte str_どくキノコ_gor_00017c6c
.4byte zero_gor_00017170
.4byte str_キノコいため_gor_00017a70
.4byte str_たんこぶ_gor_00017c78
.4byte zero_gor_00017170
.4byte str_ナンシースペシャル_gor_00017c84
"""

# Raw cookbook recipe table data - FILL THIS IN
COOKBOOK_RECIPE_DATA = """
.4byte str_キノコ_gor_00017a80
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコ_gor_00017a80
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコ_gor_00017a80
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_キノコ_gor_00017a80
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_キノコ_gor_00017a80
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコ_gor_00017a80
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコ_gor_00017a80
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコ_gor_00017a80
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_キノコいため_gor_00017a70
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコいため_gor_00017a70
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_デカデカドリンク_gor_00017ce8
.4byte str_キノコ_gor_00017a80
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_キノコ_gor_00017a80
.4byte str_キノコいため_gor_00017a70
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_アイスキャンディ_gor_00017d0c
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ハニーキノコ_gor_00017d20
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_キノコ_gor_00017a80
.4byte str_ハニーキノコ_gor_00017d20
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ハニーキノコＳ_gor_00017d30
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ハニーキノコＺ_gor_00017d40
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ハニーキノコＳ_gor_00017d30
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_ナンシーフラッペ_gor_00017d50
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_メイプルキノコ_gor_00017d64
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_キノコ_gor_00017a80
.4byte str_メイプルキノコ_gor_00017d64
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_メイプルキノコＳ_gor_00017d74
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_メイプルキノコＺ_gor_00017d88
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_メイプルキノコＳ_gor_00017d74
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_ナンシーフラッペ_gor_00017d50
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ローヤルキノコ_gor_00017d9c
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_キノコ_gor_00017a80
.4byte str_ローヤルキノコ_gor_00017d9c
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ローヤルキノコＳ_gor_00017dac
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ローヤルキノコＺ_gor_00017dc0
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ローヤルキノコＳ_gor_00017dac
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_ナンシーティー_gor_00017b98
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_キノコ_gor_00017a80
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_ハニーキノコ_gor_00017d20
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_メイプルキノコ_gor_00017d64
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_ローヤルキノコＳ_gor_00017dac
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_キノコ_gor_00017a80
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_イレカエール_gor_00017de4
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_ゆらゆらじしん_gor_00017df4
.4byte str_イレカエール_gor_00017de4
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_イレカエール_gor_00017de4
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_イレカエール_gor_00017de4
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_イレカエール_gor_00017de4
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_イレカエール_gor_00017de4
.4byte str_ゆらゆらじしん_gor_00017df4
.4byte str_かみなりゴロゴロ_gor_00017c98
.4byte str_イレカエール_gor_00017de4
.4byte str_テレサのふく_gor_00017e04
.4byte str_てきヨケール_gor_00017e14
.4byte str_イレカエール_gor_00017de4
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_イレカエール_gor_00017de4
.4byte str_てきヨケール_gor_00017e14
.4byte str_テレサのふく_gor_00017e04
.4byte str_イレカエール_gor_00017de4
.4byte str_まどわせのこな_gor_00017e24
.4byte str_しかえしのこな_gor_00017e34
.4byte str_イレカエール_gor_00017de4
.4byte str_しかえしのこな_gor_00017e34
.4byte str_まどわせのこな_gor_00017e24
.4byte str_イレカエール_gor_00017de4
.4byte str_ねむれよいこよ_gor_00017e44
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_イレカエール_gor_00017de4
.4byte str_デカデカドリンク_gor_00017ce8
.4byte str_ミニミニくん_gor_00017e54
.4byte str_イレカエール_gor_00017de4
.4byte str_カチカチコウラ_gor_00017e64
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_イレカエール_gor_00017de4
.4byte str_ミニミニくん_gor_00017e54
.4byte str_デカデカドリンク_gor_00017ce8
.4byte str_イレカエール_gor_00017de4
.4byte str_ふにゃふにゃくん_gor_00017cc0
.4byte str_カチカチコウラ_gor_00017e64
.4byte str_イレカエール_gor_00017de4
.4byte str_キノコ_gor_00017a80
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_イレカエール_gor_00017de4
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_イレカエール_gor_00017de4
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_イレカエール_gor_00017de4
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_イレカエール_gor_00017de4
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_キノコ_gor_00017a80
.4byte str_イレカエール_gor_00017de4
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_ねむれよいこよ_gor_00017e44
.4byte str_イレカエール_gor_00017de4
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_キノコ_gor_00017a80
.4byte str_イレカエール_gor_00017de4
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_イレカエール_gor_00017de4
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_イレカエール_gor_00017de4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_どくキノコ_gor_00017c6c
.4byte str_イレカエール_gor_00017de4
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_あつあつスープ_gor_00017a50
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_かみなりドッカン_gor_00017cac
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_キノコ_gor_00017a80
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_ホットドッグ_gor_00017e84
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_バクハツタマゴ_gor_00017e94
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_キノコ_gor_00017a80
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_こおりのいぶき_gor_00017cfc
.4byte str_ゆきうさぎ_gor_00017c60
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_キノコ_gor_00017a80
.4byte str_キノコいため_gor_00017a70
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_こうちゃキノコ_gor_00017eb8
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_キノコ_gor_00017a80
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_生パスタ_gor_00017bbc
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_生パスタ_gor_00017bbc
.4byte str_キノコ_gor_00017a80
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_生パスタ_gor_00017bbc
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_生パスタ_gor_00017bbc
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシーデラックス_gor_00017ed8
.4byte str_生パスタ_gor_00017bbc
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_生パスタ_gor_00017bbc
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_生パスタ_gor_00017bbc
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_生パスタ_gor_00017bbc
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ファイアキャンディ_gor_00017eec
.4byte str_おかしのもと_gor_00017bd4
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_びりびりキャンディ_gor_00017f00
.4byte str_おかしのもと_gor_00017bd4
.4byte str_まどわせのこな_gor_00017e24
.4byte str_メロメロケーキ_gor_00017f14
.4byte str_おかしのもと_gor_00017bd4
.4byte str_キノコ_gor_00017a80
.4byte str_キノコケーキ_gor_00017f24
.4byte str_おかしのもと_gor_00017bd4
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコケーキ_gor_00017f24
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコクレープ_gor_00017f34
.4byte str_おかしのもと_gor_00017bd4
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコケーキ_gor_00017f24
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_ハニーあめ_gor_00017f44
.4byte str_おかしのもと_gor_00017bd4
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_ナンシークッキー_gor_00017f50
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_ローヤルキャンディ_gor_00017f64
.4byte str_おかしのもと_gor_00017bd4
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_キノコケーキ_gor_00017f24
.4byte str_おかしのもと_gor_00017bd4
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_ナンシークッキー_gor_00017f50
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_ナンシークッキー_gor_00017f50
.4byte str_おかしのもと_gor_00017bd4
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_マンゴープディング_gor_00017f78
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_あつあつスープ_gor_00017a50
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_キノコ_gor_00017a80
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_すみぢる_gor_00017f8c
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_生パスタ_gor_00017bbc
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ファイアキャンディ_gor_00017eec
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_キノコ_gor_00017a80
.4byte str_キノコいため_gor_00017a70
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_こうちゃキノコ_gor_00017eb8
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_じわじわシロップ_gor_00017b48
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_カメまん_gor_00017fc0
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_生パスタ_gor_00017bbc
.4byte str_カメスパ_gor_00017fcc
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_すみぢる_gor_00017f8c
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_キノコ_gor_00017a80
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_つくしんぼ_gor_00017c3c
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_ピチピーチ_gor_00017c48
.4byte str_びりびりキノコ_gor_00017a60
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ピチピーチ_gor_00017c48
.4byte str_まどわせのこな_gor_00017e24
.4byte str_メロメロケーキ_gor_00017f14
.4byte str_ピチピーチ_gor_00017c48
.4byte str_キノコ_gor_00017a80
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ピチピーチ_gor_00017c48
.4byte str_スーパーキノコ_gor_00017a88
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ピチピーチ_gor_00017c48
.4byte str_ウルトラキノコ_gor_00017a98
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_ピチピーチ_gor_00017c48
.4byte str_きんきゅうキノコ_gor_00017ab8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ピチピーチ_gor_00017c48
.4byte str_しなびたキノコ_gor_00017ae0
.4byte str_キノコ_gor_00017a80
.4byte str_ピチピーチ_gor_00017c48
.4byte str_ハニーシロップ_gor_00017af0
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ピチピーチ_gor_00017c48
.4byte str_メイプルシロップ_gor_00017b14
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ピチピーチ_gor_00017c48
.4byte str_ローヤルゼリー_gor_00017b28
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ピチピーチ_gor_00017c48
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ピチピーチ_gor_00017c48
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ピチピーチ_gor_00017c48
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ピーチタルト_gor_00017fd8
.4byte str_ピチピーチ_gor_00017c48
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_スッキリドリンク_gor_00017c08
.4byte str_ヤシの実_gor_00017c54
.4byte str_ファイアフラワー_gor_00017a3c
.4byte str_ココナッツボム_gor_00017fe8
.4byte str_ヤシの実_gor_00017c54
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ヤシの実_gor_00017c54
.4byte str_トロピコマンゴー_gor_00017ba8
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ヤシの実_gor_00017c54
.4byte str_生パスタ_gor_00017bbc
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ヤシの実_gor_00017c54
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ココナツキャンディ_gor_00017ff8
.4byte str_ヤシの実_gor_00017c54
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_ヤシの実_gor_00017c54
.4byte str_ピチピーチ_gor_00017c48
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_キノコいため_gor_00017a70
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_ほしふるディナー_gor_0001800c
.4byte str_キノコいため_gor_00017a70
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_ほしふるディナー_gor_0001800c
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_キラキラおとし_gor_00017dd4
.4byte str_ほしふるディナー_gor_0001800c
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_しっぱいりょうり_gor_00018020
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ハニーキノコ_gor_00017d20
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_メイプルキノコ_gor_00017d64
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ローヤルキノコ_gor_00017d9c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ハニーキノコＳ_gor_00017d30
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_メイプルキノコＳ_gor_00017d74
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ローヤルキノコＳ_gor_00017dac
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ハニーキノコＺ_gor_00017d40
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_メイプルキノコＺ_gor_00017d88
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ローヤルキノコＺ_gor_00017dc0
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_カチカチコウラ_gor_00017e64
.4byte str_カチカチりょうり_gor_00018034
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_カチカチコウラ_gor_00017e64
.4byte str_カチカチりょうり_gor_00018034
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ナンシーデラックス_gor_00017ed8
.4byte str_カチカチコウラ_gor_00017e64
.4byte str_カチカチりょうり_gor_00018034
.4byte str_ナンシーデラックス_gor_00017ed8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ナンシーダイナマイ_gor_00018048
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_バクハツタマゴ_gor_00017e94
.4byte str_ナンシーティー_gor_00017b98
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_すみぢる_gor_00017f8c
.4byte str_ゆきうさぎ_gor_00017c60
.4byte str_あつあつスープ_gor_00017a50
.4byte str_いつまでもふたりで_gor_0001805c
.4byte str_キノコケーキ_gor_00017f24
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_キノコクレープ_gor_00017f34
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ムースケーキ_gor_00017be4
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_バクハツタマゴ_gor_00017e94
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_イカスミパスタ_gor_00018070
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_スパゲティ_gor_00017bc8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_スパゲティ_gor_00017bc8
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_スパゲティ_gor_00017bc8
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_カメスパ_gor_00017fcc
.4byte str_こうちゃキノコ_gor_00017eb8
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_すみぢる_gor_00017f8c
.4byte str_どくキノコ_gor_00017c6c
.4byte str_イレカエール_gor_00017de4
.4byte str_キノコ_gor_00017a80
.4byte str_どくキノコ_gor_00017c6c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_どくキノコ_gor_00017c6c
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_こうちゃキノコ_gor_00017eb8
.4byte str_どくキノコ_gor_00017c6c
.4byte str_カメカメはっぱ_gor_00017c1c
.4byte str_こうちゃキノコ_gor_00017eb8
.4byte str_ショコラケーキ_gor_00018080
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_マンゴープディング_gor_00017f78
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_マンゴープディング_gor_00017f78
.4byte str_ふしぎなタマゴ_gor_00017b6c
.4byte str_はつこいプディング_gor_00018090
.4byte str_はつこいプディング_gor_00018090
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ほしふるディナー_gor_0001800c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ほしふるディナー_gor_0001800c
.4byte str_フルーツパフェ_gor_00017ec8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_しれんのナベ_gor_000180a4
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_どくキノコ_gor_00017c6c
.4byte str_いつまでもふたりで_gor_0001805c
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_いつまでもふたりで_gor_0001805c
.4byte str_どくキノコ_gor_00017c6c
.4byte str_しれんのナベ_gor_000180a4
.4byte str_すみぢる_gor_00017f8c
.4byte str_じわじわキノコ_gor_00017b38
.4byte str_どくキノコ_gor_00017c6c
.4byte str_すみぢる_gor_00017f8c
.4byte str_生パスタ_gor_00017bbc
.4byte str_イカスミパスタ_gor_00018070
.4byte str_すみぢる_gor_00017f8c
.4byte str_おかしのもと_gor_00017bd4
.4byte str_ショコラケーキ_gor_00018080
.4byte str_すみぢる_gor_00017f8c
.4byte str_ムースケーキ_gor_00017be4
.4byte str_ショコラケーキ_gor_00018080
.4byte str_すみぢる_gor_00017f8c
.4byte str_スパゲティ_gor_00017bc8
.4byte str_イカスミパスタ_gor_00018070
.4byte str_キセツのオムレツ_gor_00017ea4
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_カメカメティー_gor_00017c2c
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_すみぢる_gor_00017f8c
.4byte str_カメスパ_gor_00017fcc
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_カメスパ_gor_00017fcc
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_カメスパ_gor_00017fcc
.4byte str_すみぢる_gor_00017f8c
.4byte str_イカスミパスタ_gor_00018070
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_ヤシの実_gor_00017c54
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_すみぢる_gor_00017f8c
.4byte str_イカスミパスタ_gor_00018070
.4byte str_メロメロケーキ_gor_00017f14
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ピーチタルト_gor_00017fd8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_びりびりキャンディ_gor_00017f00
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ファイアキャンディ_gor_00017eec
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ハニーあめ_gor_00017f44
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ココナツキャンディ_gor_00017ff8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ローヤルキャンディ_gor_00017f64
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ナンシークッキー_gor_00017f50
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_生パスタ_gor_00017bbc
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_キノコいため_gor_00017a70
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_キノコホイルやき_gor_00017acc
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_キノコステーキ_gor_00017aa8
.4byte str_ナンシーデラックス_gor_00017ed8
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_イカスミパスタ_gor_00018070
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_スパゲティ_gor_00017bc8
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_カメスパ_gor_00017fcc
.4byte str_ナンシーディナー_gor_00017cd4
.4byte str_ヘルスィーサラダ_gor_00017fac
.4byte str_カラリーナパスタ_gor_00017f98
.4byte str_ナンシースペシャル_gor_00017c84
.4byte str_カメまん_gor_00017fc0
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_フレッシュジュース_gor_00017b00
.4byte str_レッド・カラリン_gor_00017bf4
.4byte str_すみぢる_gor_00017f8c
.4byte str_たんこぶ_gor_00017c78
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_うちゅうしょく_gor_00017e74
.4byte str_たんこぶ_gor_00017c78
.4byte str_おうごんはっぱ_gor_00017b88
.4byte str_ナンシーデラックス_gor_00017ed8
.4byte str_きんかい_gor_000180b4
.4byte str_イレカエール_gor_00017de4
.4byte str_キノコ_gor_00017a80
.4byte str_きんかい×３_gor_000180c0
.4byte str_イレカエール_gor_00017de4
.4byte str_きんかい_gor_000180b4
.4byte str_ココナッツボム_gor_00017fe8
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_ヤシの実_gor_00017c54
.4byte str_ココナッツボム_gor_00017fe8
.4byte str_バクハツタマゴ_gor_00017e94
.4byte str_ナンシーダイナマイ_gor_00018048
.4byte str_カチカチりょうり_gor_00018034
.4byte str_ドライフラワー_gor_00017b5c
.4byte str_カチカチコウラ_gor_00017e64
"""

# Japanese to English translation dictionary for TTYD items
ITEM_TRANSLATIONS = {
    # Mushrooms
    "キノコ": "Mushroom",
    "スーパーキノコ": "Super Shroom",
    "ウルトラキノコ": "Ultra Shroom",
    "きんきゅうキノコ": "Life Shroom",
    "しなびたキノコ": "Dried Shroom",
    "びりびりキノコ": "Volt Shroom",
    "じわじわキノコ": "Slow Shroom",
    "どくキノコ": "Poison Shroom",

    # Syrups
    "ハニーシロップ": "Honey Syrup",
    "メイプルシロップ": "Maple Syrup",
    "ローヤルゼリー": "Jammin' Jelly",
    "じわじわシロップ": "Gradual Syrup",

    # Flowers
    "ファイアフラワー": "Fire Flower",
    "ドライフラワー": "Dried Bouquet",
    "おうごんはっぱ": "Golden Leaf",

    # Fruits
    "トロピコマンゴー": "Fright Mask",
    "ピチピーチ": "Peachy Peach",
    "ヤシの実": "Coconut",

    # Eggs
    "ふしぎなタマゴ": "Mystic Egg",

    # Leaves
    "カメカメはっぱ": "Turtley Leaf",
    "つくしんぼ": "Horsetail",

    # Items
    "こおりのいぶき": "Ice Storm",
    "かみなりゴロゴロ": "Thunder Bolt",
    "かみなりドッカン": "Thunder Rage",
    "キラキラおとし": "Shooting Star",
    "ゆらゆらじしん": "Earth Quake",
    "イレカエール": "Point Swap",
    "テレサのふく": "Boo's Sheet",
    "てきヨケール": "Repel Cape",
    "まどわせのこな": "Dizzy Dial",
    "しかえしのこな": "Power Punch",
    "ねむれよいこよ": "Sleepy Sheep",
    "デカデカドリンク": "Point Swap",
    "ミニミニくん": "Mini Mr. Mini",
    "カチカチコウラ": "Courage Shell",
    "ふにゃふにゃくん": "Mr. Softener",
    "ホットドッグ": "Hot Dog",
    "レッド・カラリン": "Hot Sauce",
    "生パスタ": "Fresh Pasta",
    "おかしのもと": "Cake Mix",
    "ゆきうさぎ": "Snow Bunny",
    "たんこぶ": "Whacka Bump",
    "きんかい": "Gold Bar",
    "きんかい×３": "Gold Bar x3",

    # Cooked Items
    "キノコいため": "Fried Shroom Plate",
    "キノコホイルやき": "Shroom Fry",
    "キノコステーキ": "Shroom Steak",
    "あつあつスープ": "Fire Pop",
    "フレッシュジュース": "Fresh Juice",
    "めだまやき": "Omelette Meal",
    "ナンシーティー": "Zess Tea",
    "スパゲティ": "Spaghetti",
    "ムースケーキ": "Mousse Cake",
    "スッキリドリンク": "Tasty Tonic",
    "カメカメティー": "Zess Deluxe",
    "ナンシースペシャル": "Zess Special",
    "アイスキャンディ": "Honey Candy",
    "ハニーキノコ": "Honey Shroom",
    "ハニーキノコＳ": "Honey Super",
    "ハニーキノコＺ": "Honey Ultra",
    "ナンシーフラッペ": "Zess Frappe",
    "メイプルキノコ": "Maple Shroom",
    "メイプルキノコＳ": "Maple Super",
    "メイプルキノコＺ": "Maple Ultra",
    "ローヤルキノコ": "Jelly Shroom",
    "ローヤルキノコＳ": "Jelly Super",
    "ローヤルキノコＺ": "Jelly Ultra",
    "ナンシーディナー": "Zess Dinner",
    "こうちゃキノコ": "Shroom Broth",
    "フルーツパフェ": "Fruit Parfait",
    "ナンシーデラックス": "Zess Dynamite",
    "ファイアキャンディ": "Fire Pop",
    "びりびりキャンディ": "Volt Shroom",
    "メロメロケーキ": "Love Pudding",
    "キノコケーキ": "Shroom Cake",
    "キノコクレープ": "Shroom Crepe",
    "ハニーあめ": "Honey Candy",
    "ナンシークッキー": "Zess Cookie",
    "ローヤルキャンディ": "Jelly Candy",
    "マンゴープディング": "Mango Delight",
    "すみぢる": "Ink Pasta",
    "カラリーナパスタ": "Spicy Pasta",
    "ヘルスィーサラダ": "Healthy Salad",
    "カメまん": "Koopa Bun",
    "カメスパ": "Koopasta",
    "ほしふるディナー": "Meteor Meal",
    "カチカチりょうり": "Courage Meal",
    "バクハツタマゴ": "Egg Bomb",
    "ナンシーダイナマイ": "Zess Dynamite",
    "いつまでもふたりで": "Couple's Cake",
    "うちゅうしょく": "Space Food",
    "イカスミパスタ": "Ink Pasta",
    "ショコラケーキ": "Choco Cake",
    "はつこいプディング": "Heartful Cake",
    "しれんのナベ": "Trial Stew",
    "キセツのオムレツ": "Omelette Meal",
    "ピーチタルト": "Peach Tart",
    "ココナッツボム": "Coconut Bomb",
    "ココナツキャンディ": "Coco Candy",
    "しっぱいりょうり": "Mistake",
}


def translate_item(japanese_name):
    """Translate Japanese item name to English"""
    if japanese_name is None:
        return None
    return ITEM_TRANSLATIONS.get(japanese_name, japanese_name)


def parse_simple_recipes(data_section):
    """
    Parse simple recipe table (3 entries per recipe: ingredient, result1, result2)
    Format: .4byte str_ingredient, .4byte str_result1, .4byte str_result2
    """
    recipes = []
    lines = [line.strip() for line in data_section.split('\n') if '.4byte' in line and 'str_' in line]

    # Process in groups of 3
    for i in range(0, len(lines), 3):
        if i + 2 < len(lines):
            ingredient_jp = extract_item_name(lines[i])
            result1_jp = extract_item_name(lines[i + 1])
            result2_jp = extract_item_name(lines[i + 2])

            # Only add if we have valid data (skip zero entries)
            if ingredient_jp:
                recipes.append({
                    "ingredient": translate_item(ingredient_jp),
                    "ingredient_jp": ingredient_jp,
                    "result1": translate_item(result1_jp) if result1_jp else None,
                    "result1_jp": result1_jp,
                    "result2": translate_item(result2_jp) if result2_jp else None,
                    "result2_jp": result2_jp
                })

    return recipes


def parse_cookbook_recipes(data_section):
    """
    Parse cookbook recipe table (3 entries per recipe: ingredient1, ingredient2, result)
    Format: .4byte str_ingredient1, .4byte str_ingredient2, .4byte str_result
    """
    recipes = []
    lines = [line.strip() for line in data_section.split('\n') if '.4byte' in line and 'str_' in line]

    # Process in groups of 3
    for i in range(0, len(lines), 3):
        if i + 2 < len(lines):
            ingredient1_jp = extract_item_name(lines[i])
            ingredient2_jp = extract_item_name(lines[i + 1])
            result_jp = extract_item_name(lines[i + 2])

            if ingredient1_jp and ingredient2_jp and result_jp:
                recipes.append({
                    "ingredient1": translate_item(ingredient1_jp),
                    "ingredient1_jp": ingredient1_jp,
                    "ingredient2": translate_item(ingredient2_jp),
                    "ingredient2_jp": ingredient2_jp,
                    "result": translate_item(result_jp),
                    "result_jp": result_jp
                })

    return recipes


def extract_item_name(line):
    """
    Extract item name from assembly line
    Handles formats:
    - .4byte str_キノコ_gor_00017a80
    - .4byte zero_gor_00017170
    Returns item name or None for zero entries
    """
    # Check for zero entry
    if 'zero_gor_' in line:
        return None

    # Extract between str_ and _gor_
    match = re.search(r'str_(.+?)_gor_', line)
    if match:
        return match.group(1)

    return None


def main():
    print("Recipe Parser - Paper Mario TTYD")
    print("=" * 50)

    # Parse both tables
    simple_recipes = parse_simple_recipes(SIMPLE_RECIPE_DATA)
    cookbook_recipes = parse_cookbook_recipes(COOKBOOK_RECIPE_DATA)

    # Create output structure
    output = {
        "simple_recipes": simple_recipes,
        "cookbook_recipes": cookbook_recipes,
        "statistics": {
            "total_simple_recipes": len(simple_recipes),
            "total_cookbook_recipes": len(cookbook_recipes),
            "total_recipes": len(simple_recipes) + len(cookbook_recipes)
        },
        "translation_key": ITEM_TRANSLATIONS
    }

    # Save to JSON file
    output_file = "recipes.json"
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(output, f, ensure_ascii=False, indent=2)

    print(f"\n✓ Parsed {len(simple_recipes)} simple recipes")
    print(f"✓ Parsed {len(cookbook_recipes)} cookbook recipes")
    print(f"✓ Total: {len(simple_recipes) + len(cookbook_recipes)} recipes")
    print(f"\n✓ Saved to: {output_file}")

    # Print sample recipes
    if simple_recipes:
        print("\nSample Simple Recipe:")
        print(json.dumps(simple_recipes[0], ensure_ascii=False, indent=2))

    if cookbook_recipes:
        print("\nSample Cookbook Recipe:")
        print(json.dumps(cookbook_recipes[0], ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()