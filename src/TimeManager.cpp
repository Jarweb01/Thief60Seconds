#include "TimeManager.h"
#include <random>

TimeManager::TimeManager(QObject *parent) : QObject(parent)
{
    // 1. Инициализируем генератор случайных чисел
    std::random_device rd;  // Получаем аппаратное случайное число для затравки
    std::mt19937 gen(rd()); // Инициализируем стандартный генератор Вихрь Мерсенна

    // 2. Задаем диапазон распределения
    std::uniform_int_distribution<> distr(80, 90);

    // 3. Присваиваем случайное число нашей переменной времени
    m_timeLeft = distr(gen);

    // 4. Настраиваем секундный таймер игры
    m_timer = new QTimer(this); // Выделяем память под плюсовый таймер
    m_timer->setInterval(1000); // Задаем интервал тика в 1000 миллисекунд (1 секунда)

    // Связываем сигнал таймера (timeout) со слотом (нашей функцией onTimerTick)
    connect(m_timer, &QTimer::timeout, this, &TimeManager::onTimerTick);

    m_timer->start(); // Запускаем обратный отсчет таймера
}

void TimeManager::stop() {
    if (m_timer) {
        m_timer->stop();
    }
}

void TimeManager::onTimerTick() {
    if (m_timeLeft <= 0) return;

    m_timeLeft--;
    emit timeLeftChanged();

    if (m_timeLeft == m_panicThreshold) {
        emit isTimeRunningOutChanged();
    }

    if (m_timeLeft == 0) {
        m_timer->stop();
        emit timeUp();
    }
}
