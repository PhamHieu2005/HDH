#include <Application.h>
#include <Window.h>
#include <Button.h>
#include <TextControl.h>
#include <Alert.h>
#include <LayoutBuilder.h>

enum {
    kSayHello = 'SAYH'
};

class MainWindow : public BWindow {
public:
    MainWindow()
        : BWindow(BRect(100, 100, 460, 220),
                  "Hello Haiku App",
                  B_TITLED_WINDOW,
                  B_QUIT_ON_WINDOW_CLOSE)
    {
        // Widgets
        fName = new BTextControl("name", "Your name:", "", nullptr);
        BButton* btn = new BButton("btn", "Say hello", new BMessage(kSayHello));

        // Layout (Haiku's Layout Kit)
        BLayoutBuilder::Group<>(this, B_VERTICAL, 10)
            .SetInsets(12, 12, 12, 12)
            .Add(fName)
            .AddGroup(B_HORIZONTAL, 10)
                .AddGlue()
                .Add(btn)
            .End();

        // gửi sự kiện nút về cửa sổ
        btn->SetTarget(this);
        Show();
    }

    void MessageReceived(BMessage* msg) override {
        switch (msg->what) {
            case kSayHello: {
                const char* name = fName->Text();
                BString text = "Xin chào, ";
                text << (name && *name ? name : "Haiku user") << " 👋";
                (new BAlert("Hello", text, "OK"))->Go();
                break;
            }
            default:
                BWindow::MessageReceived(msg);
        }
    }

private:
    BTextControl* fName;
};

class App : public BApplication {
public:
    App() : BApplication("application/x-vnd.demo-hellohaiku") {}
    void ReadyToRun() override { new MainWindow(); }
};

int main() {
    App app;
    app.Run();
    return 0;
}
