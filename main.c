#include <3ds.h>
#include <stdio.h>
#include <string.h>

// メモの最大保存数と文字数（日本語対応のため64に拡張）
#define MAX_MEMOS 5
#define MEMO_LENGTH 64

int main(int argc, char* argv[])
{
    // 3DSのグラフィック・画面初期化
    gfxInitDefault();

    // 上画面(Top)と下画面(Bottom)のコンソール（テキスト出力）を初期化
    PrintConsole topScreen, bottomScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    // メモ用の変数
    char memos[MAX_MEMOS][MEMO_LENGTH] = {
        "1: すれちがい通信の確認",
        "2: ゲームのバックアップ",
        "3: 未設定",
        "4: 未設定",
        "5: 未設定"
    };
    int selectedMemo = 0;

    // メインループ
    while (aptMainLoop())
    {
        // 入力（ボタン状態）をスキャン
        hidScanInput();

        // ボタンが「押された瞬間」を検知
        u32 kDown = hidKeysDown();

        // STARTボタンが押されたらアプリを終了してHBLに戻る
        if (kDown & KEY_START)
            break;

        // --- 上画面の描画処理 ---
        consoleSelect(&topScreen);
        consoleClear(); // 画面をクリア

        printf("\x1b[1;1H=== 3DS System Monitor ===");
        
        // バッテリー残量（％）の取得と表示
        u8 batteryPercent = 0;
        MCUHWC_GetBatteryLevel(&batteryPercent);
        printf("\x1b[3;1HBattery Level: %d%%", batteryPercent);

        // バッテリーが充電中かどうかの取得
        u8 isCharging = 0;
        PTMU_GetBatteryChargeState(&isCharging);
        printf("\x1b[4;1HStatus: %s", isCharging ? "Charging" : "Discharging");
        
        printf("\x1b[10;1H[START] : Exit App");


        // --- 下画面の描画処理（簡易メモ帳 & 操作UI） ---
        consoleSelect(&bottomScreen);
        consoleClear();

        printf("\x1b[1;1H=== Quick MemoPad ===");
        printf("\x1b[3;1HUse UP/DOWN to select, A to clear");

        // メモ一覧の表示
        for(int i = 0; i < MAX_MEMOS; i++) {
            if(i == selectedMemo) {
                // 選択中の行をハイライト（矢印を出す）
                printf("\x1b[%d;1H> %s", 5 + i, memos[i]);
            } else {
                printf("\x1b[%d;1H  %s", 5 + i, memos[i]);
            }
        }

        // 下画面の入力処理
        if (kDown & KEY_DOWN) {
            selectedMemo = (selectedMemo + 1) % MAX_MEMOS;
        }
        if (kDown & KEY_UP) {
            selectedMemo = (selectedMemo - 1 + MAX_MEMOS) % MAX_MEMOS;
        }
        if (kDown & KEY_A) {
            // Aボタンで選択中のメモをクリア
            strncpy(memos[selectedMemo], "Cleared (Empty)", MEMO_LENGTH);
        }

        // 画面の更新（V-Blank同期）
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // グラフィックのリソースを解放して終了
    gfxExit();
    return 0;
}
