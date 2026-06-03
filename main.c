#include <3ds.h>
#include <stdio.h>
#include <string.h>

#define MAX_MEMOS 5
#define MEMO_LENGTH 32

int main(int argc, char* argv[])
{
    // 3DSの画面初期化
    gfxInitDefault();

    // 上画面と下画面のテキスト表示（コンソール）を初期化
    PrintConsole topScreen, bottomScreen;
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    // 文字化け対策：日本語をすべてアルファベットに変更
    char memos[MAX_MEMOS][MEMO_LENGTH] = {
        "1: Check StreetPass",
        "2: Backup Game Save",
        "3: Empty Slot",
        "4: Empty Slot",
        "5: Empty Slot"
    };
    int selectedMemo = 0;

    // メインループ
    while (aptMainLoop())
    {
        // 1. 最初に入力（ボタン）をスキャン
        hidScanInput();
        u32 kDown = hidKeysDown();

        // STARTボタンで終了
        if (kDown & KEY_START)
            break;

        // 下画面の操作処理（十字キー上下で選択、Aでクリア）
        if (kDown & KEY_DOWN) {
            selectedMemo = (selectedMemo + 1) % MAX_MEMOS;
        }
        if (kDown & KEY_UP) {
            selectedMemo = (selectedMemo - 1 + MAX_MEMOS) % MAX_MEMOS;
        }
        if (kDown & KEY_A) {
            strncpy(memos[selectedMemo], "[Cleared]", MEMO_LENGTH);
        }

        // --- 2. 描画処理（ここで毎回クリアせず、位置を指定して上書きする） ---
        
        // 【上画面】
        consoleSelect(&topScreen);
        printf("\x1b[1;1H=== 3DS System Monitor ===");
        
        u8 batteryPercent = 0;
        MCUHWC_GetBatteryLevel(&batteryPercent);
        printf("\x1b[3;1HBattery Level: %d%%   ", batteryPercent); // 空白を入れて古い文字を消す

        u8 isCharging = 0;
        PTMU_GetBatteryChargeState(&isCharging);
        printf("\x1b[4;1HStatus: %s       ", isCharging ? "Charging" : "Discharging");
        
        printf("\x1b[10;1H[START] : Exit App");

        // 【下画面】
        consoleSelect(&bottomScreen);
        printf("\x1b[1;1H=== Quick MemoPad ===");
        printf("\x1b[3;1HUP/DOWN: Select | A: Clear");

        // メモ一覧を表示
        for(int i = 0; i < MAX_MEMOS; i++) {
            if(i == selectedMemo) {
                printf("\x1b[%d;1H> %-30s", 5 + i, memos[i]); // %-30sで横幅を固定してチラつき防止
            } else {
                printf("\x1b[%d;1H  %-30s", 5 + i, memos[i]);
            }
        }

        // --- 3. 画面の表示更新と、超重要な「待ち時間」 ---
        gfxFlushBuffers();
        gfxSwapBuffers();
        
        // これが3DSの画面更新（1秒間に60回）と同期させる命令。
        // これを正しい位置に入れることで、画面の点滅が完全に直ります。
        gspWaitForVBlank();
    }

    // 終了処理
    gfxExit();
    return 0;
}
