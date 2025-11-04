#include <Application.h>
#include <Window.h>
#include <Button.h>
#include <Alert.h>
#include <LayoutBuilder.h>
#include <String.h>

enum { kCellClick = 'CLIK', kNewGame = 'NEWG' };

class XOWindow : public BWindow {
public:
    XOWindow()
        : BWindow(BRect(150, 150, 480, 420), "XO - Haiku",
                  B_TITLED_WINDOW, B_QUIT_ON_WINDOW_CLOSE),
          xTurn(true)
    {
        // Nút bàn cờ 3x3
        for (int i = 0; i < 9; ++i) {
            board[i] = ' ';
            cells[i] = new BButton(nullptr, "", new BMessage(kCellClick));
            cells[i]->Message()->AddInt32("idx", i);
            cells[i]->SetTarget(this);
            cells[i]->SetEnabled(true);
            cells[i]->SetExplicitMinSize(BSize(90, 60));
            cells[i]->SetExplicitPreferredSize(BSize(100, 80));
        }

        // Nút chơi lại
        BButton* newBtn = new BButton("new", "New Game", new BMessage(kNewGame));
        newBtn->SetTarget(this);

        // Layout
        BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
            .SetInsets(12, 12, 12, 12)
            .AddGroup(B_GRID_LAYOUT, 5)
                .Add(cells[0], 0, 0).Add(cells[1], 1, 0).Add(cells[2], 2, 0)
                .Add(cells[3], 0, 1).Add(cells[4], 1, 1).Add(cells[5], 2, 1)
                .Add(cells[6], 0, 2).Add(cells[7], 1, 2).Add(cells[8], 2, 2)
            .End()
            .AddGroup(B_HORIZONTAL, 10)
                .AddGlue()
                .Add(newBtn)
            .End();

        UpdateTitle();
        Show();
    }

    void MessageReceived(BMessage* msg) override {
        switch (msg->what) {
            case kCellClick: {
                int32 idx;
                if (msg->FindInt32("idx", &idx) == B_OK && board[idx] == ' ') {
                    char mark = xTurn ? 'X' : 'O';
                    board[idx] = mark;
                    cells[idx]->SetLabel(BString() << mark);
                    cells[idx]->SetEnabled(false);

                    char win = CheckWinner();
                    if (win == 'X' || win == 'O') {
                        (new BAlert("win",
                            BString("Người chơi ") << win << " thắng!",
                            "OK"))->Go();
                        DisableBoard();
                    } else if (win == 'D') {
                        (new BAlert("draw", "Hòa!", "OK"))->Go();
                        DisableBoard();
                    } else {
                        xTurn = !xTurn;
                        UpdateTitle();
                    }
                }
                break;
            }
            case kNewGame:
                Reset();
                break;
            default:
                BWindow::MessageReceived(msg);
        }
    }

private:
    BButton* cells[9];
    char board[9];
    bool xTurn;

    void UpdateTitle() {
        SetTitle(BString("XO - Lượt của ") << (xTurn ? 'X' : 'O'));
    }

    void DisableBoard() {
        for (int i = 0; i < 9; ++i) cells[i]->SetEnabled(false);
    }

    void Reset() {
        for (int i = 0; i < 9; ++i) {
            board[i] = ' ';
            cells[i]->SetLabel("");
            cells[i]->SetEnabled(true);
        }
        xTurn = true;
        UpdateTitle();
    }

    char CheckWinner() {
        static const int lines[8][3] = {
            {0,1,2},{3,4,5},{6,7,8}, // hàng
            {0,3,6},{1,4,7},{2,5,8}, // cột
            {0,4,8},{2,4,6}          // chéo
        };
        for (auto& L : lines) {
            char a = board[L[0]], b = board[L[1]], c = board[L[2]];
            if (a != ' ' && a == b && b == c) return a;
        }
        // kiểm tra hòa
        bool full = true;
        for (int i = 0; i < 9; ++i) if (board[i] == ' ') { full = false; break; }
        return full ? 'D' : '\0';
    }
};

class XOApp : public BApplication {
public:
    XOApp() : BApplication("application/x-vnd.demo-xo") {}
    void ReadyToRun() override { new XOWindow(); }
};

int main() {
    XOApp app;
    app.Run();
    return 0;
}
