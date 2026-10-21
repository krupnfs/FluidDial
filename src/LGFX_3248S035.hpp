#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.hpp>


class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7796  _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
//----------------------------------------------------------------------  
public: 
    LGFX(void)
    {
        {            
            // Настройка управления шиной
            auto cfg = _bus_instance.config();      // получаем структуру для настройки шины。
                                                    // Настройка SPI
            cfg.spi_host   = SPI2_HOST;             // тип SPI (VSPI_HOST or HSPI_HOST)
            cfg.spi_mode   = 0;                     // SPI режим связи (0 ~ 3)
            cfg.freq_write = 80000000;              // частота отправки SPI(максимум 80MHz,80MHzを整数割値に丸め)
            cfg.freq_read  = 16000000;              // частота получения SPI
            cfg.spi_3wire  = false;                 // если используем MOSI ставим trueを設定
            cfg.use_lock   = true;                  // если используем TrancsonLock ставим true
            cfg.dma_channel=  1;                    // используем DMA или нет(1or2,0=disable)(0=DMA不使用)
            cfg.dma_channel = SPI_DMA_CH_AUTO;      // включаем DMA,效果同上
            cfg.pin_sclk   = 14;                    // пин SPI SCLK SCK
            cfg.pin_mosi   = 13;                    // пин SPI MOSI SDI
            cfg.pin_miso   = 12;                    // пин SPI MISO (-1 = disable) SDO
            cfg.pin_dc     =  2;                    // пин SPI D/C (-1 = disable) RS
            // При использовании шины SPI, общей для SD-карты, необходимо установить MISO так, чтобы его не было
            _bus_instance.config(cfg);              // Запихиваем конфигурацию
            _panel_instance.setBus(&_bus_instance); // Устанавливаем шину
        }
        {                                           // Получаем настройки для панели
            auto cfg = _panel_instance.config();    // Получаем структуру данных
            cfg.pin_cs          =    15;            // пин CS   (-1 = disable)
            cfg.pin_rst         =    -1;            // пин RST  (-1 = disable)
            cfg.pin_busy        =    -1;            // пин BUSY (-1 = disable)
            cfg.memory_width    =   320;            // Устанавливаем размер по вертикали
            cfg.memory_height   =   480;            // Устанавливаем размер по горизонтали
            cfg.panel_width     =   320;            // Размер панели по вертикали
            cfg.panel_height    =   480;            // Размер панели по горизонтали
            cfg.offset_x        =     0;            // Смещение по X
            cfg.offset_y        =     0;            // Смещение по Y
            cfg.offset_rotation =     2;            // Поворот экрана
            cfg.dummy_read_pixel=     8;            // ピクセル読出し前のダミーリードのビット数
            cfg.dummy_read_bits =     1;            // ピクセル外のデータ読出し前のダミーリードのビット数
            cfg.readable        = false;            // データ読出しが可能な場合 trueに設定
            cfg.invert          = false;            // パネルの明暗が反転場合 trueに設定
            cfg.rgb_order       = false;            // パネルの赤と青が入れ替わる場合 trueに設定 ok
            cfg.dlen_16bit      = false;            // データ長16bit単位で送信するパネル trueに設定
            cfg.bus_shared      = false;            // SDカードとバスを共有 trueに設定
            _panel_instance.config(cfg);
        }
        { 
            // Конфигурирование подсветки
            auto cfg = _light_instance.config();        // Получаем структуру конфигурации подсветки
            cfg.pin_bl = 27;                            // Пин подсветки
            cfg.invert = false;                         // инверсия подсветки true
            cfg.freq   = 44100;                         // частота PWM
            cfg.pwm_channel = 7;                        // канал PWM
            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // устанавливаем сконфигурированный профиль подсветки на панель
        }
        { 
            // Конфигурирование Touch
            auto cfg = _touch_instance.config();
            cfg.x_min      = 360;    // タッチスクリーンから得られる最小のX値(生の値) 360  222
            cfg.x_max      = 4200;   // タッチスクリーンから得られる最大のX値(生の値) 4200 3367
            cfg.y_min      = 180;    // タッチスクリーンから得られる最小のY値(生の値) 180  192
            cfg.y_max      = 3900;   // タッチスクリーンから得られる最大のY値(生の値) 3900 3732
            cfg.pin_int    = -1;     // INTが接続されているピン番号, TP IRQ
            cfg.bus_shared = true;   // 画面と共通のバスを使用している場合 trueを設定
            cfg.offset_rotation = 4; // 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定 4
            // SPI настройки для Touch
            cfg.spi_host = SPI2_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
            cfg.freq = 1000000;      // SPIクロックを設定
            cfg.pin_sclk = 14;       // SCLKが接続されているピン番号, TP CLK
            cfg.pin_mosi = 13;       // MOSIが接続されているピン番号, TP DIN
            cfg.pin_miso = 12;       // MISOが接続されているピン番号, TP DOUT
            cfg.pin_cs   = 33;       // CS  が接続されているピン番号, TP CS
            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
        }
        setPanel(&_panel_instance);// 使用するパネルをセットします。
    }
};