*This project has been created as part of the 42 curriculum by ymiyakos.*

# ft_printf

## Description

C標準ライブラリの `printf()` を再実装し、静的ライブラリ (`libftprintf.a`) としてビルドする。

glibc の `printf()` の再実装を通して可変長引数の扱い方、どのようにしてprintfが実装されているのかを学ぶ。

### 対応する変換指定子

| 指定子 | 出力 |
|--------|------|
| `%c` | 1文字 |
| `%s` | 文字列 |
| `%p` | ポインタ(16進数) |
| `%d` / `%i` | 符号付き10進数 |
| `%u` | 符号なし10進数 |
| `%x` / `%X` | 符号なし16進数(小文字 / 大文字) |
| `%%` | `%` そのもの |

### アルゴリズムと設計

#### 全体の流れ

- `format` 文字列を1文字ずつ走査し、 `%` を見つけたら `%` の次の文字を見て対応する引数を出力する
  - 変換指定子ごとの分岐は `put_args()` に集約し、走査と出力を分けることで `ft_printf()` 本体を単純に保っている
- `ft_printf()` の戻り値は出力したバイト数
- `write()` が失敗したら -1 を返し、以降の出力を打ち切る
- 出力したバイト数が INT_MAX を超える場合も -1 を返す(戻り値が `int` であり、加算でオーバーフローさせないため、加算前に `total > INT_MAX - result` で判定している)
- `%` の直後が `\0` の場合は -1 を返す
- `%s` の NULL は `(null)` 、 `%p` の NULL は `(nil)` を出力する(glibc の `printf()` に合わせた挙動)

#### 数値出力を共通関数にまとめた理由

- `%u` `%x` `%X` `%p` は「基数が違うだけの符号なし整数の出力」なので、基数を表す文字列(`"0123456789abcdef"` など)を渡す `ft_putunbr()` 1つにまとめた
  - 基数の文字列を渡す形にすることで、桁数の計算と文字の対応づけを1か所で済ませられ、大文字・小文字の違いも渡す文字列を変えるだけで表現できる
- 引数を `unsigned long` にしているのは、 `unsigned int` と `uintptr_t` (ポインタ値)のどちらも同じ関数で受けるため
- `%d` `%i` は `ft_putnbr()` で符号を出力した後に絶対値を `ft_putunbr()` へ渡す
  - 引数を `long long` にしているのは、 `INT_MIN` の符号を反転しても `int` のようにオーバーフローしないため

#### `%p` で `va_list *` を渡している理由

- ポインタは `int` や `unsigned int` とは幅が異なる(64bit環境では8バイト)ため、 `va_arg(*ap, void *)` で正しい型として取り出す必要がある
- そのため `put_args()` 側で引数を取り出さず、 `ft_putptr()` に `va_list` へのポインタごと渡して、関数内で `va_arg` を行っている
  - `va_list` を値渡しすると、呼び出された側で進めた位置が呼び出し元に反映されず、次の引数を正しく読めなくなる場合があるため、ポインタで渡している

#### `write()` を分割して呼ぶ理由

- Linux の `write()` は1回の呼び出しで最大 `0x7ffff000` バイト(= INT_MAX - 4095)までしか書き込まない
  - INT_MAX をページサイズ(4096バイト)の境界に切り下げた値であるため、 INT_MAX ではなく INT_MAX - 4095 になる
- これを超える長さの文字列は、 `ft_putstr()` で INT_MAX - 4095 バイトずつに分けて `write()` を繰り返し、 glibc の `printf()` と同様に最後まで出力する
- `write()` の戻り値が要求した長さより短い場合(部分書き込み)に備え、実際に書き込めたバイト数だけ進めて残りを再度書き込んでいる

## Instructions

### ビルド

```sh
make          # libftprintf.a をビルド
make clean    # オブジェクトファイルを削除
make fclean   # オブジェクトファイルと libftprintf.a を削除
make re       # 最初から再ビルド
```

### 使い方

```c
#include "ft_printf.h"

int main(void)
{
	ft_printf("Hello, %s! %d\n", "world", 42);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror -I include main.c libftprintf.a
```

### テスト

`test/` ディレクトリに、本物の `printf` と出力・戻り値を比較するテスターがある。

```sh
make -C test run   # 通常テスト
make -C test big   # + write の上限テスト (約2GiBのメモリを使う)
```

## Resources

- [`printf(3)` man page](https://man7.org/linux/man-pages/man3/printf.3.html)
- [`stdarg(3)` man page](https://man7.org/linux/man-pages/man3/stdarg.3.html)

### Use of AI

#### `test/`
- テストケース相談
- テスト関数の設計相談
- テスト関数の実装(全て確認済み)

#### README
- 雛形の作成(本文は自分で記述)