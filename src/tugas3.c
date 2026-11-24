#define STM32F411xE

#include "stm32f4xx.h"

int main(void)
{
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);
    RCC->APB1ENR |= (1U << 1);

    GPIOA->MODER &= ~(3U << 0);

    GPIOA->PUPDR &= ~(3U << 0);
    GPIOA->PUPDR |=  (1U << 0);

    GPIOC->MODER &= ~(3U << 26);
    GPIOC->MODER |=  (1U << 26);

    GPIOC->OTYPER &= ~(1U << 13);

    GPIOC->PUPDR &= ~(3U << 26);

    GPIOC->ODR |= (1U << 13);

    TIM3->CR1 = 0;

    TIM3->PSC = 15999;
    TIM3->ARR = 499;

    TIM3->CNT = 0;

    TIM3->EGR = (1U << 0);
    TIM3->SR = 0;

    TIM3->CR1 |= (1U << 0);

    while (1)
    {
        if ((GPIOA->IDR & (1U << 0)) == 0)
        {
            if (TIM3->SR & (1U << 0))
            {
                TIM3->SR &= ~(1U << 0);

                GPIOC->ODR ^= (1U << 13);
            }
        }
        else
        {
            GPIOC->ODR |= (1U << 13);

            TIM3->CNT = 0;

            TIM3->SR = 0;
        }
    }
}