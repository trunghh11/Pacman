#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "../Manager/LogStatus.h"
#include "Button.h"

class Menu {
    private:
        int alphaMod = 1; //đại diện cho độ trong suốt của menu.
        int arrowHSPos; // đại diện cho vị trí của mũi tên trong bảng xếp hạng cao nhất.
        int currentButtonID; //đại diện cho ID của button đang được chọn.
        int currentMenuStatus;//đại diện cho trạng thái của menu.
        int currentHTPid;//đại diện cho ID của trang hướng dẫn chơi hiện tại.

        int TOTAL_BUTTON; //đại diện cho tổng số lượng button trong menu.
        int MENU_BUTTON_WIDTH;//đại diện cho chiều rộng của button.
        int MENU_BUTTON_HEIGHT;//đại diện cho chiều cao của button.
        int baseScrPosX;//đại diện cho tọa độ x của menu.
        int baseScrPosY;//đại diện cho tọa độ y của menu.

        TextManager* scoreText = nullptr;//đại diện cho text được sử dụng để hiển thị điểm số cao nhất.
        Mix_Chunk* navigationSound = Mix_LoadWAV("Source/Assets/Sound/navigationButton.wav");//đại diện cho âm thanh được phát khi điều hướng qua các button.
        Mix_Chunk* selectionSound = Mix_LoadWAV("Source/Assets/Sound/selectButton.wav");//đại diện cho âm thanh được phát khi chọn một button.
        SDL_Texture* menuTexture;//đại diện cho hình ảnh của menu.
        SDL_Texture* howToPlayTexture[3]; //một mảng chứa 3 con trỏ tới SDL_Texture, đại diện cho 3 trang hướng dẫn chơi.
        SDL_Texture* highScoreTexture; //đại diện cho hình ảnh bảng xếp hạng điểm số cao nhất.
        SDL_Texture* arrowTexture;//đại diện cho hình ảnh mũi tên trong bảng xếp hạng cao nhất.
        std::vector<Button* > menuButton; //một vector chứa các con trỏ tới Button, đại diện cho các button trong menu.

        LogStatus* console = new LogStatus("Menu"); //một con trỏ tới LogStatus, đại diện cho trạng thái log của menu.
        bool running = false; //một boolean đại diện cho trạng thái của menu.

        SDL_Texture* loadImage(SDL_Renderer* &renderer, const std::string imgPath);
    public:
        static const int RESUME = 0; //một số nguyên đại diện cho việc tiếp tục game.
        static const int RUNNING = 1; //một số nguyên đại diện cho trạng thái game đang chạy.
        static const int PLAY_BUTTON_PRESSED = 2; //đại diện cho việc chọn button "Play".
        static const int EXIT_BUTTON_PRESSED = 3; //đại diện cho trạng thái menu khi nút "Exit" được nhấn.
        static const int HOW_TO_PLAY = 4; //đại diện cho trạng thái menu khi nút "How to Play" được nhấn.
        static const int HIGH_SCORES = 5; //đại diện cho trạng thái menu khi nút "High Scores" được nhấn.
        static const bool ON = true; //đại diện cho trạng thái bật của âm thanh.
        static const bool OFF = false; //đại diện cho trạng thái tắt của âm thanh.


        /* 
        Hàm Menu() là hàm khởi tạo đối tượng Menu, được sử dụng để thiết lập các giá trị cho các biến member của lớp. Các tham số đầu vào bao gồm:

            baseScrPosX: Vị trí x (theo pixel) của góc trái trên của menu trên màn hình.
            baseScrPosY: Vị trí y (theo pixel) của góc trái trên của menu trên màn hình.
            totalButton: Tổng số lượng nút trên menu.
            buttonWidth: Chiều rộng của nút (theo pixel).
            buttonHeight: Chiều cao của nút (theo pixel).
        */
        Menu(const int baseScrPosX, const int baseScrPosY, const int totalButton, const int buttonWidth, const int buttonHeight);


        /*Hàm ~Menu() là hàm hủy đối tượng Menu, được sử dụng để giải phóng bộ nhớ đã được cấp phát cho các biến member của lớp.*/
        ~Menu();


        /*
        Hàm init() được sử dụng để khởi tạo lớp Menu và tải hình ảnh cần thiết từ tệp ảnh đã cho. Tham số đầu vào bao gồm:

            renderer: Đối tượng SDL_Renderer để vẽ hình ảnh lên màn hình.
            imgPath: Đường dẫn đến tệp ảnh chứa hình ảnh nền và hình ảnh nút trên menu.
            buttonText: Một vector chứa các chuỗi văn bản hiển thị trên các nút trên menu.
        */
        void init(SDL_Renderer* &renderer, const std::string imgPath, std::vector<std::string> &buttonText);

        void render(SDL_Renderer* &renderer, const std::vector<std::string> &scoreData);
        /*
        được sử dụng để hiển thị các phần tử của menu lên màn hình. 
        Đầu vào của hàm là tham chiếu đến đối tượng renderer SDL_Renderer để vẽ hình ảnh trên màn hình.
        Đối số thứ hai của hàm là một vector chứa dữ liệu điểm số.
        */

        void handleEvent(SDL_Event &e, SDL_Renderer* &renderer);

        bool isRunning() const;

        int getStatus() const;

        void reOpen();

        void changeRunStatus();

        bool getSoundState() const;
};

#endif // _MENU_H_
