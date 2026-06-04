#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>

// ここにMP3/MP4デコーダーのヘッダーをインクルード
// #include "minimp3.h" 

int main(int argc, char* argv[]) {
    // 3DSの基本機能を初期化
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL); // 上画面にテキスト表示用
    ndspInit();                 // 3DSのオーディオ（NDSP）初期化

    printf("3DS Media Player\n");
    printf("Press START to exit.\n");

    // TODO: ここでSDカードからMP3/MP4ファイルを読み込む処理を記述
    // FILE *file = fopen("sdmc:/movie.mp4", "rb");

    // メインループ
    while (aptMainLoop()) {
        // 入力（ボタン）の状態をスキャン
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) {
            break; // STARTボタンでアプリ終了
        }

        /* 
         * [映像再生の処理]
         * 1. MP4から1フレーム分の動画データをデコード
         * 2. 3DSのVRAM（またはC3Dテクスチャ）にピクセルデータを書き込み
         * 3. gfxFlushBuffers() で画面を更新
         */

        /* 
         * [音声再生の処理]
         * 1. MP3からオーディオサンプルをデコード
         * 2. ndspChnWaveBufAdd() を使って3DSの音声バッファに送る
         */

        // 画面の同期とリフレッシュ（60fps制限）
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // 後処理（メモリ解放）
    ndspExit();
    gfxExit();
    return 0;
}
