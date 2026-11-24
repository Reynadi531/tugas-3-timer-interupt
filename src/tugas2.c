#define STM32F411xE

#include "stm32f4xx.h"

volatile uint8_t  sem_debounce = 0;
volatile uint8_t  pressed      = 0;
volatile uint32_t press_count  = 0;

int main(void)
{
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);
    RCC->APB1ENR |= (1U << 1);
    RCC->APB2ENR |= (1U << 14);

    GPIOA->MODER &= ~(3U << 0);
    GPIOA->PUPDR &= ~(3U << 0);
    GPIOA->PUPDR |=  (1U << 0);

    GPIOC->MODER  &= ~(3U << 26);
    GPIOC->MODER  |=  (1U << 26);
    GPIOC->OTYPER &= ~(1U << 13);
    GPIOC->PUPDR  &= ~(3U << 26);
    GPIOC->ODR    |=  (1U << 13);

    GPIOA->MODER  &= ~(3U << 10);
    GPIOA->MODER  |=  (1U << 10);
    GPIOA->OTYPER &= ~(1U << 5);
    GPIOA->PUPDR  &= ~(3U << 10);
    GPIOA->ODR    &= ~(1U << 5);

    TIM3->CR1   = (1U << 3);
    TIM3->PSC   = 15999;
    TIM3->ARR   = 19;
    TIM3->CNT   = 0;
    TIM3->EGR   = (1U << 0);
    TIM3->SR    = 0;
    TIM3->DIER |= (1U << 0);

    SYSCFG->EXTICR[0] &= ~(0xFU << 0);
    EXTI->IMR  |=  (1U << 0);
    EXTI->EMR  &= ~(1U << 0);
    EXTI->FTSR |=  (1U << 0);
    EXTI->RTSR &= ~(1U << 0);
    EXTI->PR    =  (1U << 0);

    NVIC->ISER[0] = (1U << 6);
    NVIC->ISER[0] = (1U << 29);

    while (1) {
        if (pressed) {
            pressed = 0;
            press_count++;
            GPIOC->ODR ^= (1U << 13);
        }
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0)) {
        EXTI->PR = (1U << 0);

        if (sem_debounce == 0) {
            sem_debounce = 1;
            EXTI->IMR &= ~(1U << 0);
            GPIOA->ODR |= (1U << 5);

            TIM3->CNT  = 0;
            TIM3->CR1 |= (1U << 0);
        }
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & (1U << 0)) {
        TIM3->SR &= ~(1U << 0);

        if ((GPIOA->IDR & (1U << 0)) == 0) {
            pressed = 1;
        }

        GPIOA->ODR &= ~(1U << 5);
        EXTI->PR   = (1U << 0);
        EXTI->IMR |= (1U << 0);
        sem_debounce = 0;
    }
}