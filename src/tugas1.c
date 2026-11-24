#define STM32F411xE
#include "stm32f4xx.h"

int main(void)
{
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);
    RCC->APB1ENR |= (1U << 0);
    RCC->APB1ENR |= (1U << 1);
    RCC->APB1ENR |= (1U << 2);

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

    GPIOA->MODER  &= ~(3U << 12);
    GPIOA->MODER  |=  (1U << 12);
    GPIOA->OTYPER &= ~(1U << 6);
    GPIOA->PUPDR  &= ~(3U << 12);
    GPIOA->ODR    &= ~(1U << 6);

    TIM2->PSC   = 15999;
    TIM2->ARR   = 499;
    TIM2->CNT   = 0;
    TIM2->EGR   = (1U << 0);
    TIM2->SR    = 0;
    TIM2->DIER |= (1U << 0);

    TIM3->PSC   = 1599;
    TIM3->ARR   = 2499;
    TIM3->CNT   = 0;
    TIM3->EGR   = (1U << 0);
    TIM3->SR    = 0;
    TIM3->DIER |= (1U << 0);

    TIM4->PSC   = 159;
    TIM4->ARR   = 9999;
    TIM4->CNT   = 0;
    TIM4->EGR   = (1U << 0);
    TIM4->SR    = 0;
    TIM4->DIER |= (1U << 0);

    NVIC->ISER[0] = (1U << 28);
    NVIC->ISER[0] = (1U << 29);
    NVIC->ISER[0] = (1U << 30);

    TIM2->CR1 |= (1U << 0);
    TIM3->CR1 |= (1U << 0);
    TIM4->CR1 |= (1U << 0);

    while (1) {
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1U << 0)) {
        TIM2->SR &= ~(1U << 0);
        GPIOC->ODR ^= (1U << 13);
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & (1U << 0)) {
        TIM3->SR &= ~(1U << 0);
        GPIOA->ODR ^= (1U << 5);
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & (1U << 0)) {
        TIM4->SR &= ~(1U << 0);
        GPIOA->ODR ^= (1U << 6);
    }
}