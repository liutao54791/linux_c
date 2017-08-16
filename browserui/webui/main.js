/* Copyright (C) 2015 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera Devices SDK.
 * It includes Opera proprietary information and distribution is prohibited
 * without Opera Software's prior, explicit and written consent.
 */

// FOR DEBUG ON DESKTOP
// FOR DEBUG ON DESKTOP

if (window.location.origin != "opera://browserui") {
    NON_INTEGRATED = true;
}


/**
 * Singleton History controler
 */
var SETTINGS = {
    THUMBNAIL_WIDTH: 373,//220,
    THUMBNAIL_HEIGHT: 210,//124,
    PRESSED_STATE_DURATION: 500,
    SHOW_THUMBNAILS_IN_BOOKMARKS: true,
    SHOW_THUMBNAILS_IN_TABS: true,
    SHOW_THUMBNAILS_IN_SPEED_DIAL: true,
    NOTIFICATION_DISPLAY_DURATION: 3000,
    MAX_TABS:6,
    MIN_TABS:1,
    TAB_WIDTH:300,
};
var defaultImageSrc="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAANwAAAB8CAYAAAACRt5vAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH3gYXDR8nxuZl3wAAFpBJREFUeNrtnc2S3TQQhX0dFVQIECovkaF4hLwhz8WSBW/AngVLqqCYXFZOaTTq7nP6R/YkuCqVO/fasizr8+luteTbzz//fL/dbtux9Z/Hv1+9eiXuh27ocfu+bxkbU07knN72yC7j2O73++llfPz4sWTfjDI81zYe8/j4KP4ufW63222TgDs6oAZkpLNkQIWUwZ6HvZbqh09W2Wwnu91u9DH9/mi7f/z40dwXgUkrYzwebfv+evpj7vf7JwHq99Hqeb/ftyZVAoWN6TRRwKzjmfKReqPX5gUnS8XRTu2pJwrdsY91jllZUjv0nXe2D6No3uOP6xnr3bfLrI2k79pR4PE/AprUqJEOpB2LlJsFB1JONtgrtt4diCrfrKOOT3+2A0t11vbb912FxgIKuY9HGeN9vN/v03t7lDk+4I7raEdhFmhj4R64vFBlqJH1u3U9DDhXgcxbXwuGHl5tX63De8xVFsookLN+MQIoKbsEXpvBpikaCpq0nxesDOgiUJ0FnMfXuMoW8csQE9Xa536/q+aqx8zsQZLAO9TvOH9fZhtvrBVAYRrXA1cUrIhSZgAXgS3z2CsCyMAQBc0KHEmwSYqGgGdBd7/ft7bvuxu28fvZfh6wIkrlBTlSn7MitN7gCQOvBa7XNNSO08xEBEYNREYNewAR8DToPgVNNH8NAa3/zEYxvWBlQ+z97XMInjCwSJ1OgseCUftd+s0LowabR4Elf1GC7iirSf6aZSJKoKERTaTzMmBlA1UdxKk63jPWxgZLtA4vlV9t4kqASFFGBsa+bE31NOie+HDS0MCsAyKgaeFStpNlwZUJVhVwmWamJ3MjGjn0+mQMBKhSab6aBWN/jvHzCB4CXd+2jWlIZuhgBWAZcGXCe7Z/llEHC1QPCAyIVsAD+X62Hzrg3tfJAg+Frv+/aeqmqdos0OKBDAGMhSIjeFMxuH+mX4cqFzMYHFVHJiwfUTwUwLFOs7r0ZaERzX5rlo8mjdEhoHkVjFE5FC4GLA9Uq5OyV5ibSKfMMGUZ1UFA8gA4u+ZZBHJUPEntJNPyWaYJA9vYOBETEYEuAldUsaLRyiuANutIGSYmaj6ugNCrgNJ9Hv22ETYEuic+nNURpdkE2nAA+3cEsEhZGYqXmefpPdYb6LCijmOnYUBCQMyEcBYAQRRw9rcUZe1Vazx+llXSt98nH25mSlrBERQ2z0wDj2quAKwqgyUKHju+5DmeVWcpcReBBjERZ5DMfK9ZHSzYrNkBsxzJETrtYdJYRZnByICGmnYZsHoByxxwrw6QVAdeItkZlv+VAaAGX9/pmXIk8Hp/bGYuSr8/MSmlztabkqOPx4KWAVkWYIh6ZWfMZPlcFYGK7LqjUU42bI+YjOPfWsRRyva3wJtBJcE28+saY0qO31k5mAho2ZBlABadaJsVMEHPac0biwRGokAys74ZH1DypSTFkuBDVU7LHplFMKW2bBpMWoAEhc4yU63OHgXME3jJnMV+RsYJq3heBc2a6Y5k6SMAZkU9JdhmY41SOpekcg01qRDYvKBVQxYdXPf6dhGAPJ0ZSWdCzDvLT0MhjAQ8ND8QMT9n8Gj+mRUMkf6X/DkpwvvEpNTC/yhsXtCqIWMBY9PAosnOq/1CpN4ajAiEnkFpBj42WwQJjEiD3RZ0I2ySGrZIxI0ZCM8AjfXHWF+MgcyjdtUQsdNPtGPY7HvNT5LUz/KhZtFGSYm0ezqbnY3AgUAn3YfR2vgUNDmCJai6aWbkWaBlQpaR+MyCVZmF4vG/tLxFZMDZihBa8CEBDwa8mUKiaqdB18OFTM15YlKiEUnLZ0P3OxM0BLJojuYqoLLC9NY1Sf4YEv2zAhzWgjyz83nAGyFCzcwxEqnN6p6pnAgcakKeAVs2aF4YV03NqZpek+GPSeagBZ9kZkn7zfyi/lxeU9NaFmGmXKiPNktY7s/XNHPSAgiF8kzQLMgig97ZSwWuBJVds5HN2kAGrVHwtNzFiI8nqZ21wKs2dGA9QBrjj1iqxcKGmo9ngJYBWSZc2Vn4bDIxCiCbasWAN5pxVnBlpkyI2s0UeVY3DUxpa6xZJZmSrAnJqloFaJmQXQ1Ej7IxEHoyNzTVY0xSaxZ2VO0sE1YzQ7XZFc+GBUawkMHpiAnpNR8zQfNClgHh6pW82JWokIAHAh+ierPfPfmPUbWbwSlNv0FVrgfRDJogHX+VqiFz9yKgRSHzZqJUwqdNrpwFMrzKNlMVRvVmHVkzNa0BZ0vt0OXRkSUAtSjldMb3eEImvxGZUYDC5lU1FDSPmrG/VS/XngGZFuqXlJBRNlb1NMVDfECP2qF+nWW2zkxQzax89n44yaRklC9iQq4CLQuyrMWJMjd0SQHveJsGGKt6FeAhaoeO2yHlWSo3DZpICjVTN0sxGNiyVC3DdIyAdiZgGUBKIXjpO20x1Ah42gROy7+T1E6DTjMbrUmmSJqXGjTRlM3y3zRYWdjYoAg6fMCCtnLeXMX4HDNkkLm0AWpuWmN7ln+H+naST4j6dUzY3zIrW6RTICanBVu2qlWDFpk7VwkXcx7vCz00v8mjehpQiN+kHc+Uh/h1Y9moWSn6cEymP2rOrYDNo2oIaFmTVK+0PB4T6tfgs+aXecCzzEzLt8v066wE5lnZWqpX31YN7VSMumXAhgRGWFVjQTsbMs90m2oAkeigZhJK6mCpnbU4T6Zfx/hk7BuFzPlwHt8qCzZPBJIxHz3L/WVDlhVU8bzMHrkWa3qPZm5q5hejdoyJiag10hZW1NHr8+3sDVipbFbOJrpgkJWnKZnM1tDIUTazBub4b2Vk0nNu6xrH8qTP1n2wfGh0sjM7bDPWC3kwMw/n8Vh4TZNIOlXUjET8Nc/NZFcb86iZVNaHDx9oaH799dft77//3l6/fr29fv16+/PPP9NUMWpCaQpghfStdCwt6ogonRW9lK4TCaJowwtTdZae5hrJFbOiM2FjVM3KGUXKtZ76mcGOfd+3V69ebQ8PD9u7d+9KFDBD8bS2RdVutl+W0lnlRNfMEdsPuclV88xYCK0IKWJWMtFXBrTVZuLtdtvev3+/vXv37gkE478V8DHgScsvsibmCugi/rvYl7Kc+JkdHAmSeJZB8JiQWaBFooIRxXv//v32ww8/QIrkXa4vYmJbZjvqd1dBF+1f9L1nI15s40fC/4z5isImzd1joWBBi6qOpDwHdG/fvi2tBxpAyoKO6VssXJ4IOcPH2Nf6Pr4fJuMMDM8AMRN4Qf02rxmp+Z5eVfOCVrnt+749PDxsb9++pSKRHvi8aic96FjzUqpPZGU2dPkQre9Lrtez68y4IBQU5imWUYYHtkhnWw3aDLrvv/8+3R/Lgs5SPTSQ4lFBRv086h8yKStOxJqSaGOwKx5Xw8aMWWW3+WFefvfdd+7zs5FYj9JFzUvWtGT6ylhG9kNzZzqVZ1XmaFTSa0oyK45lmJBoxC4rQiltx5CBBB0KHwNeFnReVfPW0SsYlvlIKZzHXMh4hVRGZA9NaI4o5JmgoRsKHVLPldAx5axQuQrId29HQ2cMVPlulXY267NmnK8Kum+//RYG/0zoovc3+/576iAFICEfzgOQJ7CSGTTIVLfIjV3xzm8Uuh9//HF78+ZNuG5V0J2hchX3QjNbn6z7U10Zj53s8d1YP6FC3ZDOvFr5DqU7oIv46lUPKOm7KpXzmpUZb8Hds8E5Ywb06iXHV71gMWtrrW0PDw/bN998A8NfEc3NiASjD99IBL3yvrbozfSsOFzV+ZgxN28DXuG9cP/888/2+++/08d9/fXX219//fWsbhlreUSOY9aJZMtij6+8Thq4FQO5GRHN1dexWr3+/fff7Y8//jhNMSs7cRT2inKlKUWuNxVtF9musEbIVd7VfaW2rwpufQnbrO1aRoeJ2NFn+G8vtbP+9NNP4af+b7/9dvlOmqFAkWOQNVTc/vRLNas+F3Vhtq+++upFPd0rTMJVJm3VNXw5+v7/9v92gS0NuKqn2edWpy9lq2z7FepWdQ372RVe1XhnXOP/wF/3Ientd9H67VeBxnshmcBadXjpD4crPDQq2jC7zP6as8vez24s6waP58yoQ+UN+hJU7ox+cGa5s+tF2mC6BN/Kxj/2rW7Mvnz2XF6Ve6nQVdQ7s81ZuK0H9tnw7tmFZz1Bqo+PnNNz7NWgu9/vLtisa4/A43FfPA/AzHtDjxlWdxLPhWeYlSsbET3uKtB561mhFrNzHd9VqdMZvvind9KtrlR1udoNXKFySGc+E7xK2M5Wt/GBLf2ufSc99L3m97NXWCE7ISc/XkCONp6kYmgZbAfI9uUs6F5isGQ1bCvVbYWrczCgQbxnBA3Yp1eVzzVTuQi4KHQvHTytnitgQx6yUXVj7nelK7Vn+yCVKof4chp0nk6FQvPSwDvqpLXJStiiPrunjt7ASm/NsdtecSFVKpehUBXRNxbes304CxL03kTD+TPwGEumUt2OfZcPfDOKxfphGSpnNSwzNpcJ3UvLSmFB88Km3Q/vADMCIxJAWSE8uxTdu9/vridFBK4K05INolgdj1Goo6wrw8fWL8MnRn1s5L5m+F1MdHP2MGce1rsEG9rgLCwodKwaWf5cXx+Pkx71y64CX18PFjSvKSq1ed/BK0xJBCCPBaX11Vlf6/shHaVEGsMLHdK4EaWTAimRwIHHJ3tJ5iZyfagfzZiRWWNuqCnp7V+pmSZaREbqyAwwHn/Oa15aQwZZ4H0OycrotTCqNvuMugJVsCHBFu94LD0soA3gZZzY68+hPp3HxETMTNTkeGngMQ8MxM+1PmfB5u1vFmyowrJKtx82ptXpLHNAU7moP+eBzmtiog8WBryrwsfWjwkoaaqWqWyz+xkdQ0RntVj9edandrShvSpXAR0zZICYmFonyRyzutI4XGakFbUYGH8ty4y0+hxyD7X+bynfmPLYZpXrV7vq/x5/Owo8lqo7fu+/6/c5ftfOMdt/vDitrH4f7btZedv2dKWm/rr7vy2okaX7fvnlF3MfZtWxTIg96mB1avQciAmZAZtH3VA3RLsXDYVthGgGnwSiF7q+82pAzoDIBE/6G+mwkXUzr7i+B2I2sVBZsGTAhkRI2QnSnjzcfRwrQBpSa1itcVjzMmpiMmaMZmoifyMm2ZWGA9g6Ie3BtC8Lm2Zmzr7Pgs3TZzUuGnuT+tetSirXqwSidKP5Jpmt476SiYmoHat4iOohZqDUwStWkI4Azswd8yjaClVDy2ODJJJASW3Wf980WKyTo/tY0M18RcmMtHw+yQ+bmbkskJJvZ31XDUdmQIWFLBu0K8LGJulDPtxB7djpRp+rVzlNyRjoekhQv876TSpTggsBT1K92U2Rlsm+wvLrbOfKgCwLtGx/DQmgSPB6XJkmKZqldFb0UoKu79CISYiYiBZ4s86uBTaY36RzSLBZT8sMIKMLPjGAXQm0iuAImo6GPqgaYi5KJtyogJLKWRFPy8RkfTsLZtTHQ3/TAEQgrI5MVq1j4vmdGaPzdvCoCclMM2MDac0K5yMQaDB5gimSH4aAhcBkBVcsZWMA1G5Y5RtmWPBQHzIKWRS0qKqx7SANuFvDIRKsTTMTteihpkqaKlh+3cxHQ9XOAm8W/JBMZ+sakLG2WYcZH2bWhpqY7FSh7H0zQYuYjxmwSedhhs7G/Y5jmxb+t1TOCnwgJqY0QO3N/tDAk/wvxNyMgOXpwJY6VkU6o0rnGTpgQWN8K9Rfk5TLyhtmc3CbFeafwSZBNQt8MCamFlCxwIuYmujYGpPCFR1vqx4miC5dUA1ZxE/zmpBMji7jiz4xKcfBbAY2bfiANTE13w4BjzE1EdVj1JSBSbshmYPf1WrnHZ+LgpZlPjJhf2b4gY5SzvwrDaaZP+cxMREzM0PxvPAhPtbVVS1yXnQxYOTYK4KGmKnWMh2SWdqnpanJyxZs2v6aicmonRc8VJkQ+CQA0cFtq0NXpHVFIM4Yn/MssIMoRwQ0JMpoXTs68C2V0zRQtMwTLdAigYD6dqvAQ+GTgifsoPfVlA2JrkXMzMiaoexcx0xVs+ocWUK/zfIFoyo36+gzMxOJAHrAkzq7leeIZpFIUcnoeFtm2lfW65oY88+zrmdkHh273ggLmucFMaO5OV5DY26KBJumiIiZifp3KHga7LMGs+CzALSGBrSOIdVppZJFgPEunhudsOo1Hy1TUDqWXS5fup42moTI1JsZbAh0iJlp+TSz/ftjmMAHk+HPpnGhvtvKTJOMBV+j6zayfl6FoqHXJ8HmmRt3lN1m6mJFFhHoJHAY8BBT0wufNN6Gzm9jzUpPMOUs3y0LTm9kM6JmGaAxkGnnnl1bQ8w/JoqJKpYFnkf1IsqHAshCuCoaeXYEMwJYBmTeYAhSBut79r7b+H/TfCBE5SS/DFUsZA0RVPW88EkRR2aKDRLwYDs4A+iq1wF7VM3z7gcPIBmKxoLLmrxNytqXhgiQWdpawEICR8uV1CDymp3HcYxpyEIYjT6uMjm9s50ZPy+yTPgKyBhVRGcOzM7XxgpJUT4EOtRUlADwmF0ogJ4IG1o3ZLmJyGZN7q305TzBE+/7zxkwvS/k8MJqTdPRFlGaRik18FjoNIWSVEuLBkYBZCCUjo+Yf1EgrVn5VcB5oIooF+MLroBMAlsbMrCu6VmU8vg8rl0iQSeZjpJZp5mPViQPiTRWQJhl4kWDJtlDCNEZ3VlvDaoGjPnOUlBkfE6rW0PXI9FC/9aKWxJ8bFiejTRqC/x41FTLJEEilJnArpr7lvH6Mm+wxZu76YVMAx0Zn0N81zYzI0eVY0P/SBAhGpZHB6KzOiAaGZW2x8fHsJ+XGUTJUkuPvxZ993oUWhY6LTiCvBNcXZdyBFBbolybiKo5+17YNBOUyQTxRBoz5rG9xHfHRczK6Ftts2DyrmKGvMzTgm0apbRmAFjz3WaqY+UeZgHIBj4qIo2Pj49mNDHbn1uhkJ6XaFaAtQIuqW7ofDk00VocFphB13cUNEOENfmiK1lVKRWz0jQDqfUwOstsjMLmNTdZkCLQRVYFQ2F7tmoXknw88+/YWdeeweosCDNgRFXJq5SWNbByTcuMmQVesDK/zzRZpSgk0w7T98NZPl1/EmTlK3ZOWQRCK+2KDeSwkcis4A36gIl2wBURzFXQZZitlroh75O3gixNyyiRCrLA00DTgEA7NOr3oZ17RdSO2aL+XUYdor5QJXTMNWYEdTwvlJTG554tsWBBp4EndZYMv8y6UG8nRU1AZr+oXzb6d6s29h5lwBYFa3VgB8m+0TJQmpVRonVmKxMfgcEzZseae8iAdiZ0mfBdCbAK9cuAKvp7NOPGgu1J0KT3z7QkY6vTaspTuRYjAiS72jFy81iArgJfdeTSY9ZWmqWZULH+Wj908MSHG6GaZZV431ttwVY9aZMxZz3ZIJ76nQFfBLTsiaoVwDEqlOmrWxNWoUWEogu6Rm6k9daacR8rXM6+jw3tKEygR7u+M7JPsl9FvBK+1WBpZaOwPUvtkhKSNYWITqOJNqh1vupMDq9pKQVErjbwnVlOli9XDRNyPmS9FC3RuVkJyeiE0qyGyFoWHFkLJQPQLPPwinmWq8bzKkCqTPRGJr+KKy9rU2a05RC0Jc4j20wB0HKRlyR6fc3qreIcq9Sg6jwZY23ZDxxLaCwFbEdn7jv12MGvttpUlqp5b3w1HFcY+D4L4ooE68xtFKUx8CdZgseKzP8B2FzBwXAqmnYAAAAASUVORK5CYII=";

var HISENSE_DEBUG_URL = "file:///opt/webengine/opera_dir/webui/debug/debug.html";

window.VK_LEFT = window.VK_LEFT || 37;
window.VK_RIGHT = window.VK_RIGHT || 39;
window.VK_UP = window.VK_UP || 38;
window.VK_DOWN = window.VK_DOWN || 40;
window.VK_ENTER = window.VK_ENTER || 13;
window.VK_TAB = window.VK_TAB || 9;
window.NON_INTEGRATED = window.NON_INTEGRATED || false;
window.VK_ESCAPE = window.VK_ESCAPE || 27;
window.VK_MENU = window.VK_MENU || 462;
window.VK_PLAY=window.VK_PLAY || 415;
window.VK_PAUSE=window.VK_PAUSE || 19;
window.VK_BACKSPACE=window.VK_BACKSPACE || 8;
//window.VK_BACK=window.VK_BACK || 461;
window.VK_BACK=461;
window.VK_STOP=window.VK_STOP || 413;
window.VK_FFW=window.VK_FFW || 417;
window.VK_FBW=window.VK_FBW || 412;

var i18n = function(text) {
    return i18n.translations[text] || text; // For now just return english text
};
/**
 * Initialize texts from html
 */
i18n.init = function() {
    var elements = document.querySelectorAll("[data-translation]");
    i18n.lang = window.location.GET.lang || navigator.language;

    var lang = this._fetchResourceWithFallback("lang", ".lang");

    try {
        i18n.translations = JSON.parse(lang.responseText); // load translations based on i18n.lang to a i18n.translations object
    } catch (e) {
        console.log(e)
        i18n.translations = {};
    }

    for (var i = 0, l = elements.length; i < l; i++) {
    if(elements[i].classList.toString().indexOf("fa") < 0){
            if (elements[i].nodeName == "INPUT") {
            elements[i].setAttribute("placeholder", i18n(elements[i].getAttribute("data-translation")));
            } else {
                elements[i].innerHTML = i18n(elements[i].getAttribute("data-translation"));
            }
        }
    }
    if (i18n.isRTL()) {
        var rtlStyle = document.createElement("link");
        rtlStyle.rel = 'stylesheet';
        rtlStyle.href = 'main-rtl.css';
        document.head.appendChild(rtlStyle);
        document.querySelector("#navigation").insertBefore(document.querySelector("#forward"), document.querySelector("#back"));
    }
};
i18n.loadLocalizedResource = function(folder) {
    return this._fetchResourceWithFallback(folder, ".html").responseText;
};
i18n._rtlLanguages = [//'en',
    'ar', 'arc', 'bcc', 'bqi', 'ckb', 'dv',
    'fa', 'glk', 'he', 'ku', 'mzn', 'pnb',
    'ps', 'sd', 'ug', 'ur', 'yi'
];
i18n.isRTL = function() {
    if (typeof this.__cachedIsRTLResponse == "undefined") {
        this.__cachedIsoLanguageCode = i18n.lang.replace(/-.*/, "");
        this.__cachedIsRTLResponse = i18n._rtlLanguages.indexOf(this.__cachedIsoLanguageCode) != -1;
    }
    return this.__cachedIsRTLResponse;
}

i18n.convertNumber = function(input) {
    var buffer = '';
    var offset = 0;
    var s = input.toString();
    var c = 0;

    if (this.__cachedIsoLanguageCode == 'ar') {
        offset = 1632; // Arabic numbers start at this unicode codepoint
    } else if (this.__cachedIsoLanguageCode == 'fa') {
        offset = 1776; // Persian numbers start at this unicode codepoint
    } else {
        return s;
    }

    for (var i = 0; i < s.length; i++) {
        c = parseInt(s[i]);
        buffer += isNaN(c) ? s[i] : String.fromCharCode(offset + c);
    }

    return buffer;
};

// As there is currently no way of testing if given file exists in Browser UI scope
// languages list have to be hardcoded for now, until proper 404 will be fired when
// requesting for not existing files
i18n._languageList = [
    "af", "ar", "as", "be", "bg", "bn-BD", "bs",
    "ca", "cs", "da", "de", "el", "en-GB", "en",
    "es-419", "es", "es-la", "et", "fa", "fi",
    "fr-ca", "fr", "fy", "gd", "gu", "he", "hi",
    "hr", "hu", "id", "it", "ja", "ka", "kk", "kn",
    "ko", "lt", "lv", "me", "mk", "ml", "mr", "nb",
    "nl", "nn", "or", "pa", "pl", "pt-br", "pt",
    "ro", "ru", "sk", "sl", "sr-cyrl",
    "sr", "sv", "sw", "ta", "te", "th", "tr", "uk","uz",
    "vi", "zh-cn", "zh-tw", "zu","ms","mm","sq"
];
i18n._fetchResourceWithFallback = function(folder, extension) {

    var lang = i18n.lang;

    if (this._languageList.indexOf(lang) == -1) {
        lang = lang.replace(/-.*/, "");
    }
    if (this._languageList.indexOf(lang) == -1) {
        lang = "en";
    }

    var resource = loadResource(folder + "/" + lang + extension);
    // TODO: Uncomment checking for file to have more universal way of checking for language
    /*if (resource.status == 404) {
      resource = loadResource(folder + "/en.html");
    }*/
    return resource;
};

var History = function() {
    this.init();
}
History.prototype = {
    length:0,
    init: function() {
        this.entries = [];
        this.current = 0;
    },
    add: function(url) {
        if (url == this.entries[this.entries.length - 1] || url == "") {
            return;
        }
        this.entries.splice(this.current + 1, this.entries.length); //cut out the rest
        this.entries.push(url);
        this.current++;
    },
    delete: function(url){
       if(url==""){
          return;
       }
       NativeApi.deleteHistory(url,this.deleteCallback.bind(this));
    },
    deleteCallback: function(result) {
        if (result[1] >= 0) {
//          KeyboardHandler.setFocus(document.querySelector("#elements li[tabindex]"));
            Sections.renderHistory(0);
        }
    },
    back: function() {
        if (this.current > 0) {
            this.current--;
        }
        return this.entries[this.current];
    },
    forward: function() {
        return this.entries[++this.current];
    },
    getCurrentEntry: function() {
        return this.entries[this.current];
    },
    getCurrent: function() {
        return this.current;
    },
    setCurrent: function(val) {
        this.current = val;
    },
    setCurrentEntry: function(val) {
        this.entries[this.current] = val;
    },
    getLength: function() {
//      return this.entries.length;
        return this.length;
    },
    setLength: function(val) {
//      this.entries.length = val;
        this.length = val;
    },
    getEntry: function(id) {
        return this.entries[id];
    }
};

/**
 * Singleton main view
 **/
var urlStatistics = new Array();
var Application = {
    zoom:1,
    inputUrl:"",
    init: function() {
        this.backButton = document.getElementById("back");
        this.forwardButton = document.getElementById("forward");
        this.reloadStopButton = document.getElementById("reloadStop");

        this.addTablesButton = document.getElementById("add_tables_button");
        this.closeTablesButton = document.getElementById("close_tables_button");
//      this.operaButton = document.getElementById("opera");
        this.speedDialButton = document.getElementById("open-speeddial");

        this.urlInput = document.getElementById("url");

        this.bookmarksHeaderButton = document.getElementById("bookmarks_header");
        this.historyHeaderButton = document.getElementById("history_header");
        this.bookmarkManagementButton = document.getElementById("management_exit");
        this.historyManagementButton = document.getElementById("clear_all");

        this.gotoUrlButton = document.getElementById("goToUrl");
        this.addbookmarkButton = document.getElementById("addfav");
        this.zoomButton = document.getElementById("zoom");

        this.historyDate = document.getElementById("history_date");
        this.historyDetail=document.getElementById("history_detail");

        this.setupBrowserButtonEvents();
        this.setupOptionsPopupEvents();
        this.setupPageSecurityPopupEvents();
        this.setupZoomPopupEvents();
        this.setupSettingsPopupEvents();
        this.setupHelpPopupEvents();
        this.setupAddToBookmarkPopupEvents();
        this.setupAddToSpeedDialPopupEvents();
        this.setupAdviceAddBookmarkPopupEvents();
        this.setupExitPopupEvents();
        this.setupClearHistoryPopupEvents();
        this.setupManageSpeedDialPopupEvents();
        this.setupEditSpeedDialPopupEvents();
        this.setupDeleteSpeedDialPopupEvents();
        this.setupManageBookmarkPopupEvents();
        this.setupDeleteBookmarkPopupEvents();
        this.setupConfirmDialogEvents();
        this.setupAlertDialogEvents();
        this.setupPromptDialogEvents();
        this.setupAuthenticationDialogEvents();
        this.setMarqueeElipsisInterval();

        //set config in there......just for now
        SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN = 20;
        SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE = 10;
        SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE = 50;

        //enable longpress
        SETTINGS.LONGPRESS_ENABLED = true;
        SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS = 100;

        NativeApi.getSettings(function(settings) {
            LOG("getSettings", settings);

            SETTINGS.CUSTOM_ERROR_PAGES = settings[0]["custom_error_pages"];

            SETTINGS.SHOW_THUMBNAILS_IN_TABS = settings[0]["show_thumbnails_in_tabs"];
            SETTINGS.SHOW_THUMBNAILS_IN_BOOKMARKS = settings[0]["show_thumbnails_in_bookmarks"];
            SETTINGS.SHOW_THUMBNAILS_IN_SPEED_DIAL = settings[0]["show_thumbnails_in_speeddials"];
            SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS = settings[0]["how_long_wait_for_longpress"];
            SETTINGS.LONGPRESS_ENABLED = settings[0]["longpress_enabled"];
            SETTINGS.LONGPRESS_ACCELERATION_SPEED = settings[0]["longpress_acceleration_speed"];
            SETTINGS.PLATFORM = settings[0]["platform"];
            SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN = settings[0]["scroll_margin"];
            SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE = settings[0]["mouse_step"];
            SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE = settings[0]["scroll_step"];

            SETTINGS.SPEEDDIAL_TITLE_LIMIT = parseInt(settings[0]["speeddial_title_limit"], 10) || -1;
            SETTINGS.SPEEDDIAL_URL_LIMIT = parseInt(settings[0]["speeddial_url_limit"], 10) || -1;
            SETTINGS.BOOKMARK_TITLE_LIMIT = parseInt(settings[0]["bookmark_title_limit"], 10) || -1;
            SETTINGS.BOOKMARK_URL_LIMIT = parseInt(settings[0]["bookmark_url_limit"], 10) || -1;

            var key_mapping = settings[0]["keys"];
            if (key_mapping)
                for (var key_name in key_mapping)
                    window[key_name] = window[key_name] || key_mapping[key_name];

            if (/android/i.test(SETTINGS.PLATFORM)) {
                Application.disableFeaturesInAndroid();
            }
        });
        this.getWindowSize(); // initialize
        this.reloadStopButton.classList.add("disabled");
        this.zoomButton.classList.add("disabled");
    },
    disableFeaturesInAndroid: function() {
        document.querySelector("#manage-bookmark-popup .button-move").classList.add("hide");
        document.querySelector("#manage-bookmark-popup .button-move").removeAttribute("data-default-focus");
        document.querySelector("#manage-bookmark-popup .button-delete").setAttribute("data-default-focus", true);

        document.addEventListener("mousewheel", function(e) {
            e.stopPropagation();
            e.preventDefault();
        }, false);
    },
    getWindowSize: function(func) {
        if (!this.__windowWidth) {
            NativeApi.getWindowSize(function(args) {
                Application.__windowWidth = args[0];
                Application.__windowHeight = args[1];
                Application.getWindowSize(func);
            });
        } else {
            var res = {
                width: this.__windowWidth,
                height: this.__windowHeight
            };
            if (func) {
                func(res);
            } else {
                return res;
            }
        }
    },
    /**
     * Setup events for main bar - back, forward, reload, url and settings buttons
     */
    setupBrowserButtonEvents: function() {
        this.backButton.addEventListener("click", function(e) {
            if (!/disabled/.test(this.className)) {
                Application.back();
            }
        }, true);

        this.forwardButton.addEventListener("click", function(e) {
            if (!/disabled/.test(this.className)) {
                Application.forward();
            }
        }, true);

        this.reloadStopButton.addEventListener("click", function(e) {
            if(!document.getElementById("reloadStop").classList.contains("disabled")){
                if (this.classList.contains("fa-close")) {
                    document.getElementById("reloadStop").classList.remove("fa-close");
                    document.getElementById("reloadStop").classList.add("fa-refresh");
                    var loadingStatus = document.getElementById("loading_status");
                    loadingStatus.classList.add("hide");
                    NativeApi.stopLoad();
                } else if(document.getElementById("reloadStop").classList.contains("fa-refresh")){
                    document.getElementById("reloadStop").classList.remove("fa-refresh");
                    document.getElementById("reloadStop").classList.add("fa-close");
                    NativeApi.reload();
                }
            }
        }, true);

        this.speedDialButton.addEventListener("click", function speedDialButtonHandler(e) {
            document.querySelector(".header").style.cssText += "-webkit-transition: all 0s;";
            NativeApi.loadUrl("about:blank");
            this.classList.remove("focus");
            KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
        }, true);

        this.bookmarksHeaderButton.addEventListener("click", function bookmarksHeaderButtonHandler(e) {
            HomePageManager.moveBookmarkPage();
            SpeedDial.render();
        }, true);

        this.historyHeaderButton.addEventListener("click", function historyHeaderButtonHandler(e) {
            HomePageManager.moveHistoryPage();
            Sections.renderHistory();
        }, true);

        this.bookmarkManagementButton.addEventListener("click", function bookmarkManagementButtonHandler(e) {
            KeyboardHandler.setState(KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION);
            if (SpeedDial._editing) {
                SpeedDial.disableEditing();
            } else {
                SpeedDial.enableEditing();
            }
        }, true);

        this.historyManagementButton.addEventListener("click", function historyManagementButtonHandler(e) {
            Popups.open("clear-all");
//          var focusTarget = document.querySelectorAll(".clear-all-select")[0];
            var focusTarget = document.querySelector("#clear-all-popup li")
//          focusTarget.focus();
            KeyboardHandler.setFocus(focusTarget, this.historyManagementButton);
            document.querySelector(".popup.show").style.opacity = 1;
        }, true);

        this.gotoUrlButton.addEventListener("click", function gotoUrlButtonHandler(e) {
            var forbidenList = ["/","localhost://","file://"]
            for (var forbidenItem in forbidenList) {
                if(Application.urlInput.value.indexOf(forbidenList[forbidenItem]) == 0){
                    return;
                }
            }
            if(Application.urlInput.value.indexOf("1969://") == 0){
                Application.urlInput.value = Application.urlInput.value.replace("1969://" , "file://")
            }

            if (typeof(urlStatistics[Application.urlInput.value]) == "undefined") {
                urlStatistics[Application.urlInput.value] = 1;
            } else {
                urlStatistics[Application.urlInput.value] ++;
            }
            if (urlStatistics[Application.urlInput.value] == 2) {
                NativeApi.getSpeedDialByUrl(Application.urlInput.value,function(result) {
                   if(result[0][0]==undefined){
                       document.querySelector("#advice-add-bookmark-url").innerHTML = Application.urlInput.value;
                       Popups.open("advice-add-bookmark");
                       Popups.firstPopup = true;
                       var text = document.querySelector("#advice-add-bookmark-popup").querySelector(".popup-caption").dataset.translation;
                       text += ":" + document.querySelector("#advice-add-bookmark-notice").dataset.translation;
                       text += ":" + document.querySelector("#advice-add-bookmark-url").innerHTML;
                       text += ":" + document.querySelector("#advice-add-bookmark-popup").querySelector(".button-ok").dataset.translation;
                       TTSEngine.setText( text );
                       TTSEngine.speak();
                       Application.inputUrl = currentUrl;
                       currentUrl = Application.urlInput.value;
                   }else{
                       currentUrl = Application.urlInput.value;
                       if(currentUrl != ""){
                           if (currentUrl == "hisense://debug") {
                               NativeApi.loadUrl(HISENSE_DEBUG_URL);
                               Application.urlInput.blur();
                           } else {
                               Application.inputUrl = currentUrl;
                               NativeApi.loadUrl(currentUrl);
                           }

                       }
                   }
                }.bind(this));
            }else{
                currentUrl = Application.urlInput.value;
                if(currentUrl != ""){
                    if (currentUrl == "hisense://debug") {
                        NativeApi.loadUrl(HISENSE_DEBUG_URL);
                        Application.urlInput.blur();
                    } else {
                        Application.inputUrl = currentUrl;
                        NativeApi.loadUrl(currentUrl);
                    }

                }

            }
        }, true);

        this.addTablesButton.addEventListener("click", function addTablesButtonHandler(e) {
        	TabsLine.createNewtab();
        }, true);
        this.closeTablesButton.addEventListener("click", function addTablesButtonHandler(e) {
        	TabsLine.removeNewtab(2);
        }, true);
        this.addbookmarkButton.addEventListener("click", function addbookmarkButtonHandler(e) {
            console.log("add to bookmark");
            Application.addToSpeedDialView();
        }, true);

        this.zoomButton.addEventListener("click", function zoomButtonHandler(e) {
            if(!document.getElementById("zoom").classList.contains("disabled")){
                /*
                Popups.open("zoom");
                var zoomFactor = Application.getMatchingValueOrDefault(Math.round(Tabs.getActiveTab().getZoom() * 100), [100, 150, 200], 100);
                var focusTarget = document.getElementById("button-zoom-" + zoomFactor);
                KeyboardHandler.setFocus(
                    focusTarget,
                    document.querySelector("#zoom-popup .focus")
                );
                */
                if(Application.zoom==1){
                    NativeApi.setZoom(2 * 1.5);
                    Application.zoom=2;
                    document.querySelector("#zoom").classList.remove("fa-cc-visa");
                    document.querySelector("#zoom").classList.add("fa-cc-mastercard");
                }else if(Application.zoom==2){
                    NativeApi.setZoom(3 * 1.5);
                    Application.zoom=3;
                    document.querySelector("#zoom").classList.remove("fa-cc-mastercard");
                    document.querySelector("#zoom").classList.add("fa-cc-discover");
                }else if(Application.zoom==3){
                    NativeApi.setZoom(1 * 1.5);
                    Application.zoom=1;
                    document.querySelector("#zoom").classList.remove("fa-cc-discover");
                    document.querySelector("#zoom").classList.add("fa-cc-visa");
                }
            }
        }, true);

        this.historyDate.addEventListener("click", function zoomButtonHandler(e) {
            var focusArray = Array.apply(null, document.querySelectorAll("#history_date li"));
            var currentActive = document.querySelector("#history_date .focus");
            var currentIndex = focusArray.indexOf(currentActive);
            var SECONDSPERDAY = 86400000;
            var _render_day_array = [0, 1, 2, 9];
            var _render_endtime = new Date(Date.now() - SECONDSPERDAY * _render_day_array[currentIndex]);
            _render_endtime.setHours(23, 59, 59, 999);
            if (currentIndex == 3) {
                var _render_startTime = new Date(0);
            } else {
                var _render_startTime = new Date(_render_endtime.getTime() - SECONDSPERDAY * _render_day_array[currentIndex]);
            }
            _render_startTime.setHours(0, 0, 0, 0);
            Sections.renderHistory(0, _render_startTime.getTime(), _render_endtime.getTime());
        }, true);
        this.historyDetail.addEventListener("click", function zoomButtonHandler(e) {
            var clickEvent = new Event("KeyboardEvent");
            clickEvent.keyCode=window.VK_ENTER;
            KeyboardHandler.handleKeyHistoryDetail(clickEvent);

        },true);
    },
    setMarqueeElipsisInterval: function() {
        setInterval(this._marqueeElipsisInterval.bind(this), 1000);
    },
    _marqueeElipsisInterval: function() {
        var toAnimate = document.querySelector("#elements .focus span");
        var isActive = document.querySelector("#elements.active"); // performance improvement, only animate if have focus

        var styleToAnimate = i18n.isRTL() ? "right" : "left";

        if (isActive && toAnimate && !toAnimate.classList.contains("marquee-animate")) {
            toAnimate.style.webkitTransitionDuration = (toAnimate.scrollWidth * 0.006) + "s";
            toAnimate.style[styleToAnimate] = -(toAnimate.scrollWidth - toAnimate.parentNode.offsetWidth + 14) + "px";
            toAnimate.style.width = toAnimate.scrollWidth + 5 + "px";

            toAnimate.classList.add("marquee-animate");
            toAnimate.addEventListener("webkitTransitionEnd", this._marqueeElipsisRemove, false);
        }
    },
    _marqueeElipsisRemove: function() {
        var styleToAnimate = i18n.isRTL() ? "right" : "left";
        this.removeEventListener("webkitTransitionEnd", Application._marqueeElipsisRemove, false);
        setTimeout(function() {
            this.style[styleToAnimate] = "";
            this.style.width = "";
            this.addEventListener("webkitTransitionEnd", Application._marqueeElipsisRemove2, false);
        }.bind(this), 1000);
    },
    _marqueeElipsisRemove2: function() {
        this.removeEventListener("webkitTransitionEnd", Application._marqueeElipsisRemove2, false);
        setTimeout(function() {
            this.classList.remove("marquee-animate");
        }.bind(this), 1000);
    },
    getMatchingValueOrDefault: function(attributeToCheck, values, defaults) {
        if (values.indexOf(attributeToCheck) > -1) {
            return attributeToCheck;
        } else {
            return defaults;
        }
    },
    /**
     * Open adding to bookmark view
     */
    addToBookmarkView: function() {
        var tab = Tabs.getActiveTab();
        document.querySelector("#add-to-bookmark-title").value = tab.getTitle();
        document.querySelector("#add-to-bookmark-address").value = tab.getUrl();
        Popups.open("add-to-bookmark");
    },
    addToSpeedDialView: function() {
        var tab = Tabs.getActiveTab();
        /* cancel AddToSpeedDial popup
        document.querySelector("#add-to-speed-dial-title").value = tab.getTitle() == i18n("No Name") ? "" : tab.getTitle();
        //document.querySelector("#add-to-speed-dial-address").value = tab.getUrl();
        document.querySelector("#add-to-speed-dial-address").value = Application.urlInput.value;
        Popups.open("add-to-speed-dial");
        */
        if(document.getElementById("reloadStop").classList.contains("disabled")){
            Popups.open("add-to-speed-dial");
        }else{
            var title=tab.getTitle() == i18n("No Name") ? "" : tab.getTitle();
            var url=Application.urlInput.value;
            if(url != ""){
                var currentURL = url;
                if (url == HISENSE_DEBUG_URL)
                    currentURL = "hisense://debug";
                if (currentURL.indexOf("https") >= 0){
                    var newurl = currentURL.replace("https","http");
                    SpeedDial.add(newurl, title,"");
                }else{
                    SpeedDial.add(currentURL, title,"");
                }
                Bookmarks.remove(currentURL,title);
            }
        }
    },
    setupOptionsPopupEvents: function() {
        document.querySelector("#button-add-to-bookmarks").addEventListener("click", Application.addToBookmarkView);
        document.querySelector("#button-add-to-speed-dial").addEventListener("click", Application.addToSpeedDialView);
        document.querySelector("#button-settings").addEventListener("click", function(e) {
            Popups.open("settings");
        });
        document.querySelector("#button-help").addEventListener("click", function(e) {
            Popups.open("help");
        });
        document.querySelector("#button-page-security").addEventListener("click", function(e) {
            var el = document.querySelector("#security-info-popup");
            el.classList.remove("secure", "non-secure", "expired");
            var tab = Tabs.getActiveTab();

            switch (tab.getSecureState()) {
                case 0:
                case 2:
                    el.classList.add("expired");
                    break;
                case 1:
                    el.classList.add("non-secure");
                    break;
                case 3:
                    el.classList.add("secure");
                    break
            }

            el.querySelector("#security-info-url").innerText = tab.getUrl();
            if (tab.getSecureState() == 3) {
                el.querySelector("#security-certificate").innerText = tab.getCertificate();
                el.querySelector("#security-connection").innerText = tab.getConnection();
            }

            Popups.open("security-info");
        });
        document.querySelector("#button-exit").addEventListener("click", function(e) {
            Popups.open("exit");
        });

        document.querySelector("#button-close-tab").addEventListener("click", function(e) {
            NativeApi.removeTab(Tabs.getActiveTabId());
            Popups.closeAll();
        });

        document.querySelector("#button-open-in-new-tab").addEventListener("click", function(e) {
            NativeApi.addTab(this.getAttribute("data-url"));
            Popups.closeAll();
        });

        document.querySelector("#clear-all-popup").addEventListener("click", function() {
            var _focusItem = document.querySelector("#clear-all-popup .focus");
            if (_focusItem.classList.contains("clear-all-select")) {
                var selectOptions = _focusItem.querySelectorAll("span")[1];
                if (selectOptions.classList.contains("fa-check")) {
                    selectOptions.classList.remove("fa");
                    selectOptions.classList.remove("fa-check");
                } else {
                    selectOptions.classList.add("fa");
                    selectOptions.classList.add("fa-check");
                }

            } else if (_focusItem.classList.contains("button-ok")) {
                if(document.querySelectorAll(".clear-all-select")[0].querySelectorAll("span")[1].classList.contains("fa-check")) {
                    NativeApi.clearHistory(Application.clearHistoryCallback);
                }
                if (document.querySelectorAll(".clear-all-select")[1].querySelectorAll("span")[1].classList.contains("fa-check")) {
                    //clear cookies
                    NativeApi.clearCookies();
                }
                Popups.closeAll();
            } else if (_focusItem.classList.contains("button-cancel")) {
                Popups.closeAll();
            }
        });



    },
    setupPageSecurityPopupEvents: function() {
        document.querySelector("#security-info-popup .button-ok").addEventListener("click", Popups.close.bind(Popups));
    },
    setupSettingsPopupEvents: function() {
        document.querySelector("#button-zoom").addEventListener("click", function(e) {
            Popups.open("zoom");

            var zoomFactor = Application.getMatchingValueOrDefault(Math.round(Tabs.getActiveTab().getZoom() * 100), [100, 150, 200], 100);
            var focusTarget = document.getElementById("button-zoom-" + zoomFactor);

            KeyboardHandler.setFocus(
                focusTarget,
                document.querySelector("#zoom-popup .focus")
            );
        });
        document.querySelector("#button-clear-history").addEventListener("click", function(e) {
            Popups.open("clear-history");
        });
    },
    setupZoomPopupEvents: function() {
        document.querySelector("#button-zoom-100").addEventListener("click", function(e) {
            NativeApi.setZoom(1 * 1.5);
            Popups.closeAll();
        });
        document.querySelector("#button-zoom-150").addEventListener("click", function(e) {
            NativeApi.setZoom(1.5 * 1.5);
            Popups.closeAll();
        });
        document.querySelector("#button-zoom-200").addEventListener("click", function(e) {
            NativeApi.setZoom(2 * 1.5);
            Popups.closeAll();
        });
    },
    setupHelpPopupEvents: function() {
        document.querySelector("#button-about").addEventListener("click", function(e) {
            document.querySelector("#about-popup .scrollable-content").innerHTML = i18n.loadLocalizedResource("lang/about");
            Popups.open("about");
            var a = document.querySelector("#about-popup a");
            if (a) {
                a.classList.add("focus");
            }
        });
        document.querySelector("#button-opera-help").addEventListener("click", function(e) {
            document.querySelector("#opera-help-popup .scrollable-content").innerHTML = i18n.loadLocalizedResource("lang/help");
            Popups.open("opera-help");
            var a = document.querySelector("#opera-help-popup a");
            if (a) {
                a.classList.add("focus");
            }
        });
    },
    _checkIfTitleAndUrlFieldsAreEmpty: function(url, title) {
        if (!url.trim() || !title.trim()) {
            //new Notification(i18n("Please fill in both name and address fields"));
            return true;
        }
        return false;
    },
    setupAddToBookmarkPopupEvents: function() {
        document.querySelector("#add-to-bookmark-popup .button-ok").addEventListener("click", function(e) {
            var url = document.querySelector("#add-to-bookmark-address").value;
            var title = document.querySelector("#add-to-bookmark-title").value;
            if (Application._checkIfTitleAndUrlFieldsAreEmpty(url, title)) {
                return;
            }
            Bookmarks.add(url, title);
            Popups.closeAll();
        });
        document.querySelector("#add-to-bookmark-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupAddToSpeedDialPopupEvents: function() {
        document.querySelector("#add-to-speed-dial-popup button").addEventListener("click", function(e) {
            var url = document.querySelector("#add-to-speed-dial-address").value;
            var title = document.querySelector("#add-to-speed-dial-title").value;
            if(title==""){
                title=url;
            }
            if (Application._checkIfTitleAndUrlFieldsAreEmpty(url, title)) {
                return;
            }
            var exp=/(https?:\/\/)?([\w-]+\.)+[\w-]+(\/[\w- .\/?%&=]*)?/;
            var url_check = new RegExp(exp);
            if(url_check.test(url)){
                if (url.indexOf("https") >= 0 ){
                    var newurl = url.replace("https","http");
                    SpeedDial.add(newurl, title,document.querySelector("#add-to-speed-dial-address").getAttribute("thumbNailStr"));
                }else{
                    SpeedDial.add(url, title,document.querySelector("#add-to-speed-dial-address").getAttribute("thumbNailStr"));
                }
                Bookmarks.remove(url,title);
                Popups.closeAll();
            }else{
                document.querySelector("#url_tip").style.visibility="visible";
                setTimeout(function() {
                    document.querySelector("#url_tip").style.visibility="hidden";
                }, 3000);
            }
            document.querySelector("#add-to-speed-dial-title").value="";
            document.querySelector("#add-to-speed-dial-address").setAttribute("thumbNailStr","");
        });
        //document.querySelector("#add-to-speed-dial-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupAdviceAddBookmarkPopupEvents: function() {
        var buttonArray = document.querySelectorAll("#advice-add-bookmark-popup button");
        buttonArray[0].addEventListener("click", function(e) {
            var url = document.querySelector("#advice-add-bookmark-url").innerHTML;
            var title = document.querySelector("#advice-add-bookmark-url").innerHTML;
            if (Application._checkIfTitleAndUrlFieldsAreEmpty(url, title)) {
                return;
            }
            UrlAdvice.change(url,title);
            UrlAdvice.add();
            Popups.closeAll();
        });
        buttonArray[1].addEventListener("click", Popups.close.bind(Popups));
    },
    setupExitPopupEvents: function() {
        document.querySelector("#exit-popup .button-ok").addEventListener("click", function() {
            window.close();
        });
        document.querySelector("#exit-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupClearHistoryPopupEvents: function() {
        document.querySelector("#clear-history-popup .button-ok").addEventListener("click", function() {
            Popups.closeAll();
            NativeApi.clearHistory(Application.clearHistoryCallback);
        });
        document.querySelector("#clear-history-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupManageSpeedDialPopupEvents: function() {
        document.querySelector("#manage-speed-dial-popup .button-move").addEventListener("click", function() {
            Popups.close();
            SpeedDial.enableMoveState();
        });

        document.querySelector("#manage-speed-dial-popup .button-edit").addEventListener("click", function() {
            Popups.close();
            //document.querySelector("#edit-speed-dial-address").value = document.querySelector(".speeddials .focus").getAttribute("data-url");
            //document.querySelector("#edit-speed-dial-title").value = document.querySelector(".speeddials .focus").getAttribute("data-title");
            document.querySelector("#edit-speed-dial-address").value = Popups.prefocused.getAttribute("data-url");
            document.querySelector("#edit-speed-dial-title").value = Popups.prefocused.getAttribute("data-title");
            document.querySelector("#speeddial-caption").innerText = "Edit Speed Dial";
            Popups.open("edit-speed-dial", function() {
                document.querySelector("#speeddial-caption").innerText = i18n("Select a Speed Dial entry to edit");
            });
            //Popups.prefocused=null;
        });

        document.querySelector("#manage-speed-dial-popup .button-delete").addEventListener("click", function() {
            Popups.close();
            document.querySelector("#speeddial-caption").innerText = "Edit Speed Dial";
            Popups.open("delete-speed-dial", function() {
                document.querySelector("#speeddial-caption").innerText = i18n("Select a Speed Dial entry to edit");
            });
        });
    },
    setupEditSpeedDialPopupEvents: function() {
        document.querySelector("#edit-speed-dial-popup .button-ok").addEventListener("click", function(e) {
            var url = document.querySelector("#edit-speed-dial-address").value;
            var title = document.querySelector("#edit-speed-dial-title").value;
            if (Application._checkIfTitleAndUrlFieldsAreEmpty(url, title)) {
                return;
            }
            //SpeedDial.edit(document.querySelector(".speeddials .focus").getAttribute("data-url"), url, title);
            SpeedDial.edit(Popups.prefocused.getAttribute("data-url"), url, title);
            Popups.prefocused=null;
            Popups.closeAll();
        });
        document.querySelector("#edit-speed-dial-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupDeleteSpeedDialPopupEvents: function() {
        document.querySelector("#delete-speed-dial-popup .button-ok").addEventListener("click", function() {
            //SpeedDial.remove(document.querySelector(".speeddials .focus").getAttribute("data-url"));
            SpeedDial.remove(Popups.prefocused.getAttribute("data-url"));
            Popups.prefocused=null;
            Popups.close();
        });
        document.querySelector("#delete-speed-dial-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupManageBookmarkPopupEvents: function() {
        document.querySelector("#manage-bookmark-popup .button-move").addEventListener("click", function() {
            Sections.setEditingState(true);
            Popups.close();
        });

        document.querySelector("#manage-bookmark-popup .button-delete").addEventListener("click", function() {
            Popups.close();
            Popups.open("bookmark-delete");
        });
    },
    setupDeleteBookmarkPopupEvents: function() {
        document.querySelector("#bookmark-delete-popup .button-ok").addEventListener("click", function() {
            var index = +document.querySelector(".bookmark.focus").getAttribute("data-id");
            var url = Bookmarks.getEntry(index).url;
            var title = Bookmarks.getEntry(index).title;
            Bookmarks.remove(url, title);
            Popups.close();
        });
        document.querySelector("#bookmark-delete-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    setupConfirmDialogEvents: function() {
        document.querySelector("#confirm-popup .button-cancel").addEventListener("click", function() {
            Popups.closeStackingPopup(false);
        });
        document.querySelector("#confirm-popup .button-ok").addEventListener("click", function() {
            Popups.closeStackingPopup(true);
        });
    },
    setupAlertDialogEvents: function() {
        document.querySelector("#alert-popup .button-close").addEventListener("click", Popups.closeStackingPopup.bind(Application));
    },
    setupPromptDialogEvents: function() {
        document.querySelector("#prompt-popup .button-ok").addEventListener("click", Popups.closeStackingPopup.bind(Application));
    },
    setupAuthenticationDialogEvents: function() {
        document.querySelector("#authentication-popup .button-ok").addEventListener("click", Popups.close.bind(Popups));
        document.querySelector("#authentication-popup .button-cancel").addEventListener("click", Popups.close.bind(Popups));
    },
    back: function() {
        if (Tabs.getActiveTab().History.getLength() <= 0) {
            return;
        }
        //var url = Tabs.getActiveTab().History.back();
        NativeApi.historyMove(0);
        //      Tabs.getActiveTab().setUri()
        var history = Tabs.getActiveTab().History;
        this.prepareForwardButtons(history.getCurrent(), history.getLength());
    },
    forward: function() {
        var history = Tabs.getActiveTab().History;
        if (history.getCurrent() + 1 >= history.getLength()) {
            return;
        }
        NativeApi.historyMove(1);
        this.prepareForwardButtons(history.getCurrent(), history.getLength());
    },
    prepareForwardButtons: function(index, length) {
        if (index + 1 === length) {
            this.forwardButton.classList.add("disabled");
        } else {
            this.forwardButton.classList.remove("disabled");
        }
        if (index === 0) {
            this.backButton.classList.add("disabled");
        } else {
            this.backButton.classList.remove("disabled");
        }
        var focused = document.querySelector("#navigation .focus");
        if(focused == null) {//#
            
        } else {
            if (focused.classList.contains("disabled")) {
                focused.classList.remove("focus");
                document.querySelector("#navigation [tabindex]:not(.disabled)").classList.add("focus");
            }
        }
    },
    newEntry: function(url) {
        Tabs.getActiveTab().History.add(url);
        this.updateUrl(url, Tabs.getActiveTabId());
        changeUrl(url);
        this.backButton.className = "fa fa-chevron-left";
        this.backButton.setAttribute("tabindex", "1");
        KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
        return url;
    },
    onNewEntry: function(url, index, length, tab_id) {
        var history = Tabs.getActiveTab().History;
        history.setLength(length);
        history.setCurrent(index);
        this.updateUrl(url, tab_id);
        Sections.renderHistory(0);
    },
    updateUrl: function(url, tab_id) {
        LOG("updateURL = " + url + " tab_id = " + tab_id);
        if(url.indexOf(Application.inputUrl)>0){
            Bookmarks.add(url, url);
            Application.inputUrl="";
        }
        if (url == "about:blank") {
            document.querySelector(".speeddial").classList.add("show");
            if (KeyboardHandler.state == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION || KeyboardHandler.state == null)
                KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);

           NativeApi.show();
        } else {
            document.querySelector(".speeddial").classList.remove("show");
            // TODO change this window size;
            // If we were in initial "showing speed-dial" mode and url started loading,
            // we should turn into page navigation mode. This happens when BrowserUI
            // window is created with initial url.
            if (KeyboardHandler.state == KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION || KeyboardHandler.state==KeyboardHandler.states.POPUP_NAVIGATION || KeyboardHandler.state==KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION ||KeyboardHandler.state == null)
                KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);


//          if (KeyboardHandler.state != KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
                var loadingStatus = document.getElementById("loading_status");
                loadingStatus.classList.remove("hide");
//          }
        }

//      Only one tab
//      var tab_shown = document.querySelector(".tab[data-id=\"" + tab_id + "\"]");
//      if (tab_shown) {
//          var oldOpened = document.querySelector(".opened");
//          if (oldOpened) {
//              oldOpened.classList.remove("opened");
//          }
//          tab_shown.classList.add("opened");
//      }

        if (url == "about:blank")
            this.urlInput.value = "";
        else if (!isCustomErrorPages(url))
            this.urlInput.value = url;

        var securityIcon = document.querySelector("#security-icon");
        if (this.urlInput.value === "") {
//          this.urlInput.parentNode.classList.add("empty-url-input");
            securityIcon.classList.remove("fa-globe");
            securityIcon.classList.add("fa-search");
        } else {
//          this.urlInput.parentNode.classList.remove("empty-url-input");
            securityIcon.classList.remove("fa-search");
            securityIcon.classList.add("fa-globe");
        }
        NativeApi.getCertificateInfo(function(info) {
            Tabs.getTab(tab_id).setCertificate(info[0] + " " + info[1]);
            Tabs.getTab(tab_id).setConnection(info[2]);
        });
        this.updateSecurity(Tabs.getTab(tab_id).getSecureState(), Tabs.getTab(tab_id).getEvSsl(), tab_id);
        var history = Tabs.getTab(tab_id).History;
        this.prepareForwardButtons(history.getCurrent(), history.getLength());
    },
    updateSecurity: function(state, is_ev_ssl, tab) {
        var el = document.querySelector("#security-icon");
//      el.className = "";
//      if (state == 0 || state == 2) {
//          el.className = "expired";
//      }
//      if (state == 3) {
//          if (is_ev_ssl) {
//              el.className = "ev_secure";
//          } else {
//              el.className = "secure";
//          }
//      }
        Tabs.getTab(tab).setSecureState(state);
        Tabs.getTab(tab).setEvSsl(is_ev_ssl);
    },
    clearHistoryCallback: function(result) {
        if (result[0] == 0) {
            new Notification(i18n("History cleared"));
            Sections.renderHistory(0);
        } else {
            new Notification(i18n("There was a problem while clearing history"));
        }
    },
    clearAllNavFocus:function(){
        var allFocused=document.querySelector("#navigation").querySelectorAll(".focus");

        for(var i=0;i<allFocused.length;i++){
            allFocused[i].classList.remove("focus");
        }
    }
};

var Popups = {
    prefocused:null,
    _openedPopup: null,
    _popupList: [],
    init: function() {
        document.addEventListener("click", function(e) {
            var rootNode = e.target;
            while (rootNode != document.body && !rootNode.classList.contains("popup")) {
                rootNode = rootNode.parentNode;
            }
            if (rootNode != document.body) {
                if (e.target == rootNode.firstElementChild && !rootNode.classList.contains("only-button-can-close")) {
                    Popups.close();
                }
            }

        }, true);
    },
    open: function(id, closeCallback, modalData) {
        // Make sure that we got proper state on stack of popups
        this._handleCurrentPopupDuringOpen(modalData, closeCallback);

        // If there is no openedPopup - setup proper keyboard handling
        if (!this._openedPopup) {
            this._previousState = KeyboardHandler.getState();
            if (this._previousState == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
                NativeApi.hideMouseCursor();
            }
            KeyboardHandler.setTemporaryState(KeyboardHandler.states.POPUP_NAVIGATION);
            if (window.innerHeight != 1080) {
                NativeApi.setWindowSize(0, 0, window.innerWidth, 1080,null);
            }
        }

        // create new popup entry
        var newPopup = {
            id: id,
            callback: closeCallback,
            element: document.getElementById(id + "-popup"),
            modalData: modalData
        };

        // In case there is already modal popup shown and new comes in, move it to stack
        // otherwise just show it
        if (this._openedPopup && this._openedPopup.modalData) {
            this._popupList.unshift(newPopup);
        } else {
            this._openedPopup = newPopup;
            this._show(this._openedPopup.element);
        }
    },

    // Logic that handles what happens with currentOpened popup during opening new one:
    // 1. If a new popup is modal one, close all other on stack.
    // 2. Otherwise hide current popup, push it on stack and later on show newlyOpened
    _handleCurrentPopupDuringOpen: function(modalData, closeCallback) {
        if (this._openedPopup) {
            if (modalData) {
                this.closeAll();
            } else {
                this._popupList.push(this._openedPopup);
                this._hide(this._openedPopup.element);
            }
        }
    },
    // Modal dialogs might contain some dynamic content, update them before showing
    _handleOpeningModalDialog: function() {
        if (this._openedPopup.modalData && this._openedPopup.modalData.type == "dialog") {
            this._setupStackingPopup(this._openedPopup.modalData);
        }
    },


    _show: function(element, retainOldFocus) {
        if(KeyboardHandler.navTimer){
            clearTimeout(KeyboardHandler.navTimer);
        }
        this._oldActive = document.querySelector(".active");
        this._handleOpeningModalDialog();
        if (this._oldActive) {
            this._oldActive.classList.remove("active");
        }
        element.classList.add("show", "active");
        NativeApi.gainFocus();

        if (retainOldFocus) {
            return;
        }

        var oldFocus = element.querySelector(".focus");
        var newFocus = element.querySelector("[data-default-focus]");
        if (newFocus) {
            if(oldFocus){
                oldFocus.classList.remove("focus");
            }
            {
                var index=0;
                var nodeArray=newFocus.parentNode.querySelectorAll(".focus");
                while(index<nodeArray.length){
                    try{
                        nodeArray[index].classList.remove("focus");
                    }catch(e){
                        console.log(e);
                    }
                    index++;
                }
            }
            newFocus.classList.add("focus");
        }
    },
    _hide: function(element) {
        element.classList.remove("show", "active");
        if (this._oldActive) {
            this._oldActive.classList.add("active");
        }
    },

    close: function(dialog_result, dialog_value) {
        if(!this._openedPopup){
            return;
        }
        if(this._openedPopup.id=="advice-add-bookmark"){
            NativeApi.loadUrl(currentUrl);
        }
        this._hide(this._openedPopup.element);
        if (typeof this._openedPopup.callback == "function") {
            this._openedPopup.callback(this._openedPopup.modalData, dialog_result, dialog_value);
        }
        var previousPopup = this._popupList.pop();
        if (previousPopup) {
            this._show(previousPopup.element, true);
            this._openedPopup = previousPopup;
        } else {
            this._openedPopup = null;
            KeyboardHandler.setTemporaryState(this._previousState);
            if (this._previousState == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
                NativeApi.showMouseCursor();
            }
        }
        NativeApi.blurFocus();
        KeyboardHandler.setNavTimer();
    },
    // Close All method will close all popups except the modal ones - those has to be closed manualy by user, or other action
    closeAll: function() {
        while (this._openedPopup && !this._openedPopup.modalData) {
            this.close();
        }
    },
    // Public method to close stacking popups from OK/Cancel button events
    closeStackingPopup: function(result) {
        Popups.close(!!result, document.querySelector("#prompt-input").value);
    },
    // Public method to show prompt dialog
    showPromptPopup: function(text, default_prompt, tab_id) {
        this._showStackingPopup({
            view: "prompt",
            text: text,
            default_prompt: default_prompt,
            tab_id: tab_id
        });
    },
    // Public method to show confirm dialog
    showConfirmPopup: function(text, tab_id) {
        this._showStackingPopup({
            view: "confirm",
            text: text,
            tab_id: tab_id
        });
    },
    // Public method to show alert dialog
    showAlertPopup: function(text, tab_id) {
        this._showStackingPopup({
            view: "alert",
            text: text,
            tab_id: tab_id
        });
    },
    // Public method to show authentication dialog
    showAuthenticationDialog: function(tab_id) {
        document.querySelector("#authentication-username").value = "";
        document.querySelector("#authentication-password").value = "";
        Popups.open("authentication", this._authenticationDialogCallback, {
            type: "authentication",
            tab_id: tab_id
        });
    },

    // Helper method to prepare type and display stacking popup
    _showStackingPopup: function(popupData) {
        popupData.type = "dialog";
        Popups.open(popupData.view, this._stackingDialogCallback, popupData);
    },
    _stackingDialogCallback: function(popupData, dialog_result, dialog_value) {
        NativeApi.dialogClosed(popupData.tab_id, dialog_result, dialog_value);
    },
    // Ensures that dialogs get actual set of data for current popup to be shown
    _setupStackingPopup: function(popupData) {
        document.querySelector("#prompt-popup .popup-description").innerText = document.querySelector("#confirm-popup .popup-description").innerText = document.querySelector("#alert-popup .popup-description").innerText = popupData.text;
        document.querySelector("#prompt-input").value = popupData.default_prompt || "";
    },
    // callback for authentication dialog that can be also called when BACK is pressed
    // Used for sending proper authenticationReceived values back to native part
    _authenticationDialogCallback: function(data) {
        if (document.querySelector("#authentication-popup .button-ok.focus")) {
            NativeApi.authenticationReceived(
                data.tab_id,
                true,
                document.querySelector("#authentication-username").value,
                document.querySelector("#authentication-password").value
            );
        } else {
            NativeApi.authenticationReceived(
                data.tab_id,
                false,
                "",
                ""
            );
        }
    }

};

/**
 * Singleton Keyboard handler
 */
var speeddialNewfocus;
var KeyboardHandler = {
    state: null,
    navTimer: 0,
    states: {
        PAGE_NAVIGATION: 0,
        MENU_NAVIGATION: 1,
        GRID_NAVIGATION: 2,
        VIRTUAL_MOUSE_NAVIGATION: 3,
        SPEED_DIAL_PAGE_NAVIGATION: 4,
        POPUP_NAVIGATION: 5,
        BOOKMARKS_EDIT_NAVIGATION: 6,
        BOOKMARKS_HISTORY_NAVIGATION: 7,
        HISTORY_CLEAR_ALL_NAVIGATION: 8,
        HISTORY_DATE_NAVIGATION: 9,
        HISTORY_DETAIL_NAVIGATION: 10,
        FULL_SCREEN_VIDEO_PLAY:11,
        TAB_NAVIGATION:12
    },
    init: function() {
        document.querySelector(".background-dummy").classList.add("show");
        window.addEventListener("keydown", KeyboardHandler.handleKeyProxy.bind(this), true);
        window.addEventListener("keyup", KeyboardHandler.handleKeyUp.bind(this), true);
        this._pressCount = 1;
    },

    getState: function() {
        return KeyboardHandler.state;
    },

    setTemporaryState: function(state) {
        this.state = state;
    },
    setNavTimer:function(){
            if(this.navTimer){
                clearTimeout(this.navTimer);
            }

            this.navTimer=setTimeout(function(){
                if(!document.querySelector(".speeddial").classList.contains("show")&&KeyboardHandler.getState()==KeyboardHandler.states.MENU_NAVIGATION){
                NativeApi.getVirtualKeyBoardStatus(function(isVirtualKeyBoardShown){
                    if(isVirtualKeyBoardShown[0]){
                        return;
                    }
                    console.log("state1="+KeyboardHandler.getState());
                    KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
                    console.log("state2="+KeyboardHandler.getState());
                });
                }
                /*
                var a=document.querySelector(".line.active");
                var b= a.querySelector(".focus");
                b.classList.remove("focus");
                if (document.activeElement.nodeName == "INPUT") {
                    document.activeElement.blur();
                }
                */

            }.bind(this),8000);

    },
    isUserInteractingWithKeyboard: function() {
        return this._userInteractingWithKeyboard;
    },

    setState: function(state, focusItem) {
        var clear = document.querySelector(".active");
        for (var i in this.states) {
            if (this.states[i] == state) {
                break;
            }
        }
        console.log("Setting keyboard state: " + i);
        if (clear) {
            clear.classList.remove("active");
        }
        if (state == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
            NativeApi.showMouseCursor();
            console.log("destroyThumbnailWindow");
            NativeApi.destroyThumbnailWindow();
        } else if (state == KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY) {
            NativeApi.hideMouseCursor();
        } else {
            NativeApi.hideMouseCursor();
            if (window.innerHeight != 1080) {
                NativeApi.setWindowSize(0, 0, window.innerWidth, 1080,function(e){
                    document.querySelector(".header").style.cssText += "-webkit-transition: all 1s;";
                    document.querySelector(".header").classList.remove("anihide");
                    //document.querySelector("#status-bar").classList.remove("hide");
                });
            }
        }
        if (state == KeyboardHandler.states.POPUP_NAVIGATION) {
            return;
        }
        /*
        if (state != KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION &&
            state != KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION) {
            SpeedDial.disableEditing();
        }
        */
        if (state == KeyboardHandler.states.MENU_NAVIGATION) {
            NativeApi.show();
            NativeApi.gainFocus();
            //document.querySelector(".header").classList.remove("anihide");
//          document.querySelector("#status-bar").classList.remove("anihide");

            /* Set active focus to URL bar */
            document.querySelector("#navigation").classList.add("active");
            if (this.state == KeyboardHandler.states.MENU_NAVIGATION) {
                document.querySelector("#navigation .focus").classList.remove("focus");
            }
            if (focusItem) {
                if(focusItem.parentNode){
                    var focusedChildren=focusItem.parentNode.querySelectorAll(".focus");
                    var index=0;
                    while(index<focusedChildren.length){
                        focusedChildren[index].classList.remove("focus");
                        index++;
                    }
                }
                focusItem.classList.add("focus");
                if (focusItem.nodeName == "INPUT") {
                    focusItem.focus();
                    focusItem.select();
                }
            }
            else {
                document.querySelector("#url").classList.add("focus");
                document.querySelector("#url").focus();
                document.querySelector("#url").select();
            }
            /*
            setTimeout(function() {
                document.querySelector("#status-bar").classList.remove("hide");
            }, 10);
            */
            /* Delay adding css class so it can be animated after window is shown */
//          setTimeout(function() {
//              document.querySelector(".header").classList.add("show");
//          }, 10);
        }else if(state == KeyboardHandler.states.TAB_NAVIGATION){
            NativeApi.show();
            NativeApi.gainFocus();
            document.querySelector("#tables").classList.add("active");
            document.getElementById("table-0").classList.add("focus");
        } else if (state == KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION) {
            //NativeApi.gainFocus();
            //NativeApi.show();
            HomePageManager.moveBookmarkPage();
//          if (state != this.state) {
//              new Notification(i18n("Move <b>Up</b> to access the address bar"));
//          }
            document.querySelector(".speeddial").classList.add("active");
            try {
                document.querySelector(".speeddial .focus").classList.remove("focus");
            } catch (e) {
                //TODO handle the exception
            }

            document.querySelector(".speeddial li").classList.add("focus");
            document.querySelector(".speeddial").classList.add("show");
            document.querySelector(".header").classList.remove("show");
            document.querySelector(".background-dummy").classList.remove("show");
            //document.querySelector(".speeddial li").classList.add("focus");
        } else if (state == KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION) {
            NativeApi.gainFocus();
            document.querySelector("#bookmark_page").classList.add("active");
            document.querySelector("#management_exit").classList.add("focus");
            //document.querySelector("#management_exit").classList.add("focus");
            //document.querySelector("#management_exit").classList.remove("speeddial");
        } else if (state == KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION) {
            NativeApi.gainFocus();
            document.querySelector("#speeddial-header").classList.add("active");
            if (HomePageManager.state == HomePageManager.states.BOOKMARK_PAGE) {
                document.querySelector("#bookmarks_header").classList.add("focus");
            } else {
                document.querySelector("#history_header").classList.add("focus");
            }

        } else if (state == KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION) {
            document.querySelector("#history_page").classList.add("active");
            document.querySelector("#clear_all").classList.add("focus");
        } else if (state == KeyboardHandler.states.HISTORY_DATE_NAVIGATION) {
            document.querySelector("#history_date").classList.add("active");
//          document.querySelector("#history_date li").classList.add("focus");
            var dateArray = document.querySelectorAll("#history_date li");
            var focusedDate = null;
            for (var index = 0; index < dateArray.length; index++) {
                if (dateArray[index].classList.contains("focused")) {
                    focusedDate = dateArray[index];
                    break;
                }
            }
            if (focusedDate == null) {
                focusedDate = dateArray[0];
            }
            focusedDate.classList.add("focus");
        } else if (state == KeyboardHandler.states.HISTORY_DETAIL_NAVIGATION) {
            document.querySelector("#history_detail").classList.add("active");
            document.querySelector("#history_detail li").classList.add("focus");
        }else if(state==KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY){
            PlayControl._button_array[1].classList.add("focus");
        }else {
            //TODO:
            /* Remove class show that animates bar upwards */
//          document.querySelector(".header").classList.remove("show");
            document.querySelector(".background-dummy").classList.remove("show");
            document.querySelector(".speeddial").classList.remove("show");
            document.querySelector(".header").style.cssText += "-webkit-transition: all 0s;";
            //document.querySelector("#status-bar").classList.add("hide");
            document.querySelector(".header").classList.add("anihide");
            setTimeout(function() {
                NativeApi.setWindowSize(0, 0, window.innerWidth, 4,null);
                NativeApi.focusMainWindow();
                //NativeApi.blurFocus();
            }, 50);
//          document.querySelector("#status-bar").classList.add("anihide");
        }
        this.state = state;
    },
    enableButton: function(but) {
        but.classList.remove("hide");
        but.setAttribute("tabindex", 1);
    },
    disableButton: function(but) {
        but.classList.add("hide");
        but.removeAttribute("tabindex");
    },
    handleNonLongPress: function() {
        this.handleKey(this._lastKeyDownEvent, false);
    },
    handleKeyUp: function(e){
        if(e.keyCode == VK_ENTER ){
           if(document.querySelector(".line.active") && document.querySelector(".line.active").querySelector(".focus")){
               var a = document.querySelector(".line.active").querySelector(".focus");
               if(a.value!="" && a.id == "url"){
                   Application.gotoUrlButton.click();
               }
           }
        }
    },
    handleKeyProxy: function(e) {
        var isInputElement = !!document.querySelector(".active input.focus"); // Longpress breaks android input focusing so it need to be filtered
        if (SETTINGS.LONGPRESS_ENABLED && !isInputElement && (KeyboardHandler.getState() == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION || this._longPressStopped)) {
            if (this._lastKeyDown + SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS > Date.now() && this._lastKeyDownEvent.keyCode == e.keyCode) {
                clearTimeout(this._longPressCheck);
                if (!this._longPressStopped) {
                    this._pressCount++;
                    this.handleKey(e, true);
                }
            } else {
                this._bumpShown = false;
                this._pressCount = 1;
                if (e.keyCode !== VK_ENTER) {
                    if (this._lastKeyDownEvent && this._lastKeyDownEvent.keyCode == VK_ENTER && this._lastKeyDown + SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS > Date.now() && !this._longPressStopped) {
                        this.handleKey(this._lastKeyDownEvent, false);
                    }
                    this.handleKey(e, false);
                } else {
                    clearTimeout(this._longPressCheck);
                    this._longPressCheck = setTimeout(this.handleNonLongPress.bind(this), SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS);
                }
                this._longPressStopped = false;
            }
        } else {
            if (!(this._lastKeyDownEvent && this._lastKeyDownEvent.keyCode == VK_ENTER && e.keyCode == VK_ENTER && this._lastKeyDown + SETTINGS.HOW_LONG_WAIT_FOR_LONGPRESS > Date.now())) {
                this.handleKey(e);
            }
        }
        this._lastKeyDownEvent = e;
        this._lastKeyDown = Date.now();
    },

    _userInteractedWithKeyboard: function() {
        this._userInteractingWithKeyboard = true;
        clearTimeout(this._userInteractingWithKeyboardTimeout);
        this._userInteractingWithKeyboardTimeout = setTimeout(function() {
            this._userInteractingWithKeyboard = false;
        }.bind(this), 1000);
    },

    _returnMatchingValueInArray: function(array, value) {
        if (typeof array == "number") {
            return array;
        }
        return (array || []).indexOf(value) > -1 ? value : -1;
    },

    _handleSpecialKey: function(e) {
        switch (e.keyCode) {
            case window.VK_TAB:
                if (this.state != KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
                    e.preventDefault();
                }
                break;
            case this._returnMatchingValueInArray(window.VK_BROWSER_ZOOM_OUT, e.keyCode):
                var currentSupportedZoom = Application.getMatchingValueOrDefault((Tabs.getActiveTab().getZoom() * 100) | 0, [100, 150, 200], 100);
                document.querySelector("#button-zoom-" + Math.max(100, currentSupportedZoom - 50)).click();
                break;
            case this._returnMatchingValueInArray(window.VK_BROWSER_ZOOM_IN, e.keyCode):
                var currentSupportedZoom = Application.getMatchingValueOrDefault((Tabs.getActiveTab().getZoom() * 100) | 0, [100, 150, 200], 100);
                document.querySelector("#button-zoom-" + Math.min(200, currentSupportedZoom + 50)).click();
                break;
            case this._returnMatchingValueInArray(window.VK_BLUE, e.keyCode):
                if(KeyboardHandler.getState()==KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY){
                    PlayControl._progressbar.style.width = "0%";
                    PlayControl._hide();
                    PlayControl._hideLoading();
                    PlayControl._removeAllFocus();
                }
                Popups.closeAll();
                Application.clearAllNavFocus();
                if(document.activeElement.tagName.toLowerCase() == "input"){
                    document.activeElement.blur();
                }
                document.querySelector("#open-speeddial").click();
                break;
            case this._returnMatchingValueInArray(window.VK_RED, e.keyCode):
                if(KeyboardHandler.getState()==KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY){
                    PlayControl._button_array[3].click();
                }else{
                    document.querySelector("#back").click();
                }
                break;
            case this._returnMatchingValueInArray(window.VK_GREEN, e.keyCode):
                document.querySelector("#forward").click();
                break;
            case this._returnMatchingValueInArray(window.VK_YELLOW, e.keyCode):
                if(KeyboardHandler.getState()==KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY){
                    return;
                }
                NativeApi.show();
                document.querySelector("#addfav").click();
                break;
            case this._returnMatchingValueInArray(window.VK_CHANNEL_UP, e.keyCode):
                /*
                NativeApi.getScroll(function(scrollPos) {
                    var x = parseInt(scrollPos[0], 10);
                    var y = parseInt(scrollPos[1], 10);
                    if (y != 0)
                        NativeApi.scroll(0, y - 50);
                });
                */
                NativeApi.scrollUp();
                break;
            case this._returnMatchingValueInArray(window.VK_CHANNEL_DOWN, e.keyCode):
                /*
                NativeApi.getScroll(function(scrollPos) {
                    var x = parseInt(scrollPos[0], 10);
                    var y = parseInt(scrollPos[1], 10);
                    NativeApi.scroll(0, y + 50);
                });
                */
                NativeApi.scrollDown();
                break;
            case this._returnMatchingValueInArray(window.VK_BROWSER_TAB_NEXT, e.keyCode):
                var tabs = Tabs.getAllTabs();
                var activated_id = Tabs.getActiveTabId();
                var first, found, next;
                for (var i in tabs) {
                    if (!first) {
                        first = i;
                    }
                    if (found && !next) {
                        next = i;
                    }
                    if (i == activated_id) {
                        found = i;
                    }
                }
                NativeApi.activateTab(next ? next : first);
                break;
            case this._returnMatchingValueInArray(window.VK_BROWSER_TAB_PREV, e.keyCode):
                var tabs = Tabs.getAllTabs();
                var activated_id = Tabs.getActiveTabId();
                var last, found, prev;
                for (var i in tabs) {
                    if (i == activated_id) {
                        found = i;
                    }
                    if (!found) {
                        prev = i;
                    }
                    last = i;
                }
                NativeApi.activateTab(prev ? prev : last);
                break;
            case this._returnMatchingValueInArray(window.VK_BROWSER_URL_BAR, e.keyCode):
                if (KeyboardHandler.getState() !== KeyboardHandler.states.POPUP_NAVIGATION) {
                    KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION);
                }
                break;
        }
    },

    _swapLeftRightKeys: function(e) {
        var dummyEvent = new Event("KeyboardEvent");
        switch (KeyboardHandler.state) {
            case KeyboardHandler.states.MENU_NAVIGATION:
            case KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION:
            case KeyboardHandler.states.POPUP_NAVIGATION:
            case KeyboardHandler.states.HISTORY_DATE_NAVIGATION:
            case KeyboardHandler.states.HISTORY_DETAIL_NAVIGATION:
                if (e.keyCode == VK_LEFT) {
                    dummyEvent.keyCode = VK_RIGHT;
                    return dummyEvent
                }
                if (e.keyCode == VK_RIGHT) {
                    dummyEvent.keyCode = VK_LEFT;
                    return dummyEvent
                }
        }
        return e;
    },


    handleKey: function(e, longpress) {
        this._userInteractedWithKeyboard();
        if (document.activeElement && document.activeElement.nodeName != "INPUT") {
            document.activeElement.blur();
        }
        if (this.state != KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
            NativeApi.hideMouseCursor();
        }
        this._handleSpecialKey(e);
        if (i18n.isRTL()) {
            e = this._swapLeftRightKeys(e);
        }
        var multiplicaction = 1.8;
    if(longpress == true) {
            multiplicaction = 5.4;
        }

        switch (KeyboardHandler.state) {
            case KeyboardHandler.states.PAGE_NAVIGATION:
                this.handleKeyPageNavigation(e);
                break;
            case KeyboardHandler.states.MENU_NAVIGATION:
                this.handleKeyMenu(e);
                break;
            case KeyboardHandler.states.GRID_NAVIGATION:
                break;
            case KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION:
                //this.handleKeyVirtualMouse(e, Math.min(10, Math.ceil(this._pressCount / (SETTINGS.LONGPRESS_ACCELERATION_SPEED || 3))), longpress);
                this.handleKeyVirtualMouse(e,multiplicaction,longpress);
                break;
            case KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION:
                this.handleKeySpeedDial(e);
                break;
            case KeyboardHandler.states.POPUP_NAVIGATION:
                this.handleKeyPopup(e);
                break;
            case KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION:
                this.handleKeyBookmarksEdit(e);
                break;
            case KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION:
                this.handleKeyBookmarksHistory(e);
                break;
            case KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION:
                this.handleKeyClearAll(e);
                break;
            case KeyboardHandler.states.HISTORY_DATE_NAVIGATION:
                this.handleKeyHistoryDate(e)
                break;
            case KeyboardHandler.states.HISTORY_DETAIL_NAVIGATION:
                this.handleKeyHistoryDetail(e);
                break;
            case KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY:
                this.hanleKeyVideoControl(e);
                break;
            case KeyboardHandler.states.TAB_NAVIGATION:
                this.hanlekeyNewTab(e);
                break;
        }
        setTimeout(function() {
            switch (KeyboardHandler.state) {
                case KeyboardHandler.states.PAGE_NAVIGATION:
                    break;
                case KeyboardHandler.states.MENU_NAVIGATION:
                    var navgationbar = document.querySelector("#navigation");
                    var focused = navgationbar.querySelector(".focus");
                    var text = focused.dataset.translation;
                    if(focused.id == "url"){
                        if(focused.value != ""){
                            text = focused.value;
                        }
                    }
                    TTSEngine.setText( text );
                    break;
                case KeyboardHandler.states.GRID_NAVIGATION:
                    break;
                case KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION:
                    break;
                case KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION:
                    {
                        var text = document.querySelector(".focus").dataset.title;
                        if(!text){
                            text = document.querySelector("#addfav").dataset.translation;
                        }
                        TTSEngine.setText(text);
                    }

                    break;
                case KeyboardHandler.states.POPUP_NAVIGATION:
                    {
                        var popup = document.querySelector(".popup.active.show");
                        var focused = popup.querySelector(".focus");
                        var text = "";
                        switch(popup.id){
                            case "clear-all-popup":
                                text = focused.children[0].dataset.translation;
                            break;
                            case "add-to-speed-dial-popup":
                                if(focused.id == "add-to-speed-dial-address"){
                                    if(focused.value != ""){
                                        text = focused.value;
                                    }else{
                                        text = document.querySelector("#address_span").dataset.translation;
                                    }
                                } else if(focused.id == "add-to-speed-dial-button" ) {
                                    text = document.querySelector("#addfav").dataset.translation;
                                }
                                else {
                                    text = focused.dataset.url;
                                }
                            break;
                            case "advice-add-bookmark-popup":
                                if(!Popups.firstPopup)
                                    text = focused.dataset.translation;
                                Popups.firstPopup = false;
                            break;
                        }
                        TTSEngine.setText(text);
                    }
                    break;
                case KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION:
                    TTSEngine.setText( document.querySelector("#management_exit").innerText);
                    break;
                case KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION:
                    TTSEngine.setText( document.querySelector(".focus").dataset.translation);
                    break;
                case KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION:
                    TTSEngine.setText( document.querySelector(".focus").dataset.translation);
                    break;
                case KeyboardHandler.states.HISTORY_DATE_NAVIGATION:
                    TTSEngine.setText( document.querySelector(".focus").dataset.translation);
                    break;
                case KeyboardHandler.states.HISTORY_DETAIL_NAVIGATION:
                    {
                        var focused = document.querySelector(".focus")
                        var text;
                        if(focused.classList.toString().indexOf("add_history_to_bookmark") > -1){
                            text = "add history to bookmark";
                        }else if(focused.classList.toString().indexOf("del_history") > -1)
                        {
                            text = "delete history"
                        }else{
                            text = focused.children[0].innerHTML + "," + focused.children[2].innerHTML + "," + focused.children[3].innerHTML;
                        }
                        TTSEngine.setText(text);
                    }
                    break;
                case KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY:
                    break;
            }
            TTSEngine.speak();
        }.bind(this), 100);

    },
    hanlekeyNewTab: function hanlekeyNewTab(e){
        this.setNavTimer();
        var lines = Array.apply(null, document.querySelectorAll(".line"));
        var currentLine = document.querySelector(".line.active");
        if (!currentLine) return;
        var lineIndex = lines.indexOf(currentLine);
        console.log(lineIndex);
        var currentLineItems = Array.apply(null, currentLine.querySelectorAll("[tabindex]:not(.hide)"));
        var currentFocusItem = currentLine.querySelector(".focus");
        var currentSlectItem = currentLine.querySelector(".select");
        var currentIndex = currentLineItems.indexOf(currentFocusItem);
        if (currentLine.querySelectorAll("[tabindex]:not(.hide)").length >= 8) {
        	document.getElementById("add_tables_button").classList.add("disabled");
        }

        if (!currentFocusItem) {
            currentLineItems[0] && KeyboardHandler.setFocus(currentLineItems[0]);
            currentFocusItem = currentLine.querySelector(".focus");
        }

        switch(e.keyCode){
            case window.VK_LEFT:
            if (currentIndex > 0) {
                    this.setFocus(currentLineItems[currentIndex - 1], currentFocusItem);
                }
                e.preventDefault();
                break;
            case window.VK_RIGHT:
                if (currentIndex < currentLineItems.length - 1) {
                    this.setFocus(currentLineItems[currentIndex + 1], currentFocusItem);
                } 
                e.preventDefault();
                break;
            case window.VK_DOWN:
                 if (document.querySelector(".speeddial").classList.contains("show")) {
                        this.setState(KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION);
                    } else {
                        this.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
                    }
                    currentFocusItem.classList.remove("focus");
                    e.preventDefault();
                break;
            case window.VK_UP:
                this.setState(KeyboardHandler.states.MENU_NAVIGATION);
                currentFocusItem.classList.remove("focus");
                break;
            case window.VK_ENTER:
                if ((!currentFocusItem.classList.contains("fa-plus-circle")) && (!currentFocusItem.classList.contains("fa-times-circle"))) {
                    NativeApi.activateTab(currentFocusItem.getAttribute("tabindex"));
                }
                if (currentFocusItem.classList.contains("fa-plus-circle")){
                	NativeApi.addTab("about:blank");
                }
                if (currentFocusItem.classList.contains("fa-times-circle")){
                	if (currentSlectItem != undefined) {
                        NativeApi.removeTab(currentSlectItem.getAttribute("tabindex"));
                }
            }
                break;
            case window.VK_BACK:
                if(Application.backButton.classList.contains("disabled")){
                    sendCommand("exit_browser");
                }else{
                    Application.backButton.click();
                }
                break;
        }
    },
    hanleKeyVideoControl:function hanleKeyVideoControl(e){
        switch(e.keyCode) {
            case window.VK_PAUSE:
            case window.VK_PLAY:
            case window.VK_FFW:
            case window.VK_FBW:
            case window.VK_STOP:
                PlayControl._fullscreen_player.classList.remove("browser-player-hide");
                break;
        }
        if (PlayControl._fullscreen_player.classList.contains("browser-player-hide")) {
            PlayControl._playerbarAutoHide();
            e.stopPropagation();
            e.preventDefault();
            return;
        }
        PlayControl._playerbarAutoHide();
        var curActive = PlayControl._button_array[0].parentElement.querySelector(".focus");
        var curIndex = Array.apply(null,PlayControl._button_array).indexOf(curActive);
        switch (e.keyCode) {
            case window.VK_LEFT:
                if (curIndex > 0) {
                    curActive.classList.remove("focus");
                    PlayControl._button_array[curIndex - 1].classList.add("focus");
                }
                break;
            case window.VK_RIGHT:
                if (curIndex < PlayControl._button_array.length - 1) {
                    curActive.classList.remove("focus");
                    PlayControl._button_array[curIndex + 1].classList.add("focus");
                }
                break;
            case window.VK_ENTER:
                curActive.click();
                break;
            case window.VK_BACK:
                PlayControl._button_array[3].click();
                break;
            case window.VK_PAUSE:
                if(PlayControl._play_pause == false){
                    PlayControl._button_array[1].click();
                }
                break;
            case window.VK_PLAY:
                if(PlayControl._play_pause == true){
                    PlayControl._button_array[1].click();
                }
                break;
            case window.VK_FFW:
                PlayControl._button_array[2].click();
                break;
            case window.VK_FBW:
                PlayControl._button_array[0].click();
                break;
            case window.VK_STOP:
                PlayControl._stop();
                break;
        }
        e.stopPropagation();
        e.preventDefault();
    },
    handleKeyBookmarksEdit: function handleKeyBookmarksEdit(e) {
        var focused = document.querySelector("#management_exit");
        switch (e.keyCode) {
            case window.VK_DOWN:
                //focused.blur();
                focused.classList.remove("focus");
                KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
                break;
            case window.VK_UP:
                //focused.blur();
                focused.classList.remove("focus");
                KeyboardHandler.setState(KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION);
                break;
            case window.VK_ENTER:
                focused.click();
                e.stopPropagation();
                e.preventDefault();
                break;
            case window.VK_BACK:
                if (SpeedDial._editing) {
                    SpeedDial.disableEditing();
                }else{
                    if(Application.backButton.classList.contains("disabled")){
                        sendCommand("exit_browser");
                        }else{
                            Application.backButton.click();
                        }
                }
                break;

        }
    },

    handleKeyBookmarksHistory: function handleKeyBookmarksHistory(e) {
        var focused = document.querySelector("#speeddial-header .focus");
        switch (e.keyCode) {
            case window.VK_UP:
                focused.classList.remove("focus");
                focused.classList.add("focused");
                KeyboardHandler.setState(KeyboardHandler.states.TAB_NAVIGATION);
                break;
            case window.VK_DOWN:
                focused.classList.remove("focus");
                focused.classList.add("focused");
                if (HomePageManager.state == HomePageManager.states.BOOKMARK_PAGE) {
                    KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
                } else if (HomePageManager.state == HomePageManager.states.HISTORY_PAGE) {
                    KeyboardHandler.setState(KeyboardHandler.states.HISTORY_DATE_NAVIGATION);
                }
                break;
            case window.VK_LEFT:
            case window.VK_RIGHT:
                focused.classList.remove("focused");
                if (HomePageManager.state == HomePageManager.states.BOOKMARK_PAGE) {
                    this.setFocus(document.querySelector("#history_header"), focused);
                    HomePageManager.moveHistoryPage();
                    Sections.renderHistory();
                } else {
                    this.setFocus(document.querySelector("#bookmarks_header"), focused);
                    HomePageManager.moveBookmarkPage();
                    SpeedDial.render();
                }
                break;
            case window.VK_ENTER:
                focused.click();
                e.stopPropagation()
                e.preventDefault();
                break;
            case window.VK_BACK:
                if(Application.backButton.classList.contains("disabled")){
                    sendCommand("exit_browser");
                }else{
                    Application.backButton.click();
                }
                break;
        }
    },

    handleKeyClearAll: function handleKeyClearAll(e) {
        var focused = document.querySelector("#clear_all");
        switch (e.keyCode) {
            case window.VK_DOWN:
                focused.classList.remove("focus");
                KeyboardHandler.setState(KeyboardHandler.states.HISTORY_DATE_NAVIGATION);
                break;
            case window.VK_UP:
                focused.classList.remove("focus");
                KeyboardHandler.setState(KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION);
                break;
            case window.VK_ENTER:
                focused.click();
                break;
            default:
                break;
        }
    },

    handleKeyHistoryDate: function handleKeyHistoryDate(e) {
        var focusArray = Array.apply(null, document.querySelectorAll("#history_date li"));
        var currentActive = document.querySelector("#history_date .focus");
        var currentIndex = focusArray.indexOf(currentActive);
        //24 hours
        var SECONDSPERDAY = 86400000;
        //0 for today, 1 for yesterday, 2 for lastweek ?, 9 for before
        var _render_day_array = [0, 1, 2, 9];
        var _render_day_length = [_render_day_array[1] - _render_day_array[0] - 1,
                                  _render_day_array[2] - _render_day_array[1] - 1,
                                  _render_day_array[3] - _render_day_array[2] - 1];
        switch (e.keyCode) {
            case window.VK_UP:
                if (currentIndex > 0) {
                    this.setFocus(focusArray[currentIndex - 1], currentActive);
                    currentActive.classList.remove("focused");
                    currentIndex -= 1;
                    var _render_endtime = new Date(Date.now() - SECONDSPERDAY * _render_day_array[currentIndex]);
                    _render_endtime.setHours(23, 59, 59, 999);
                    if (currentIndex == 3) {
                        var _render_startTime = new Date(0);
                    } else {
                        var _render_startTime = new Date(_render_endtime.getTime() - SECONDSPERDAY * _render_day_length[currentIndex]);
                    }
                    _render_startTime.setHours(0, 0, 0, 0);
                    console.log("HistoryStartTime = " + _render_startTime.toString());
                    console.log("HistoryEndTime = " + _render_endtime.toString());
                    Sections.renderHistory(0, _render_startTime.getTime(), _render_endtime.getTime());
                } else {
                    currentActive.classList.remove("focus");
                    currentActive.classList.add("focused");
                    this.setState(KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION);
                }
                break;
            case window.VK_DOWN:
                if (currentIndex < focusArray.length - 1) {
                    this.setFocus(focusArray[currentIndex + 1], currentActive);
                    currentActive.classList.remove("focused");
                    currentIndex += 1;
                    var _render_endtime = new Date(Date.now() - SECONDSPERDAY * _render_day_array[currentIndex]);
                    _render_endtime.setHours(23, 59, 59, 999);
                    if (currentIndex == 3) {
                        var _render_startTime = new Date(0);
                    } else {
                        var _render_startTime = new Date(_render_endtime.getTime() - SECONDSPERDAY * _render_day_length[currentIndex]);
                    }
                    _render_startTime.setHours(0, 0, 0, 0);
                    console.log("HistoryStartTime = " + _render_startTime.toString());
                    console.log("HistoryEndTime = " + _render_endtime.toString());
                    Sections.renderHistory(0, _render_startTime.getTime(), _render_endtime.getTime());
                } else {
                    //Back to first history
                    this.setFocus(focusArray[0], currentActive);
                    currentActive.classList.remove("focused");
                    var _render_endtime = new Date(Date.now());
                    _render_endtime.setHours(23, 59, 59, 999);
                    var _render_startTime = new Date(_render_endtime.getTime());
                    _render_startTime.setHours(0, 0, 0, 0);
                    console.log("HistoryStartTime = " + _render_startTime.toString());
                    console.log("HistoryEndTime = " + _render_endtime.toString());
                    Sections.renderHistory(0, _render_startTime.getTime(), _render_endtime.getTime());
                }
                break;
            case window.VK_RIGHT:
                //No history in history detail
                if (document.querySelectorAll("#history_detail li.hide").length == 9) {
                    break;
                } else {
                    currentActive.classList.remove("focus");
                    currentActive.classList.add("focused");
                    this.setState(KeyboardHandler.states.HISTORY_DETAIL_NAVIGATION);
                }
                break;
            default:
                break;
        }
    },

    handleKeyHistoryDetail: function handleKeyHistoryDetail(e) {
        var focusArray = Array.apply(null, document.getElementById("history_detail").children);
        var currentActive = document.querySelector("#history_detail .focus");
        var currentIndex = focusArray.indexOf(currentActive);

        switch (e.keyCode) {
            case window.VK_UP:
                if (currentIndex > 2) {
                    this.setFocus(focusArray[currentIndex - 3], currentActive);
                } else if (Sections.getHistoryIndex() > 0) {
                    Sections.renderHistory(Sections.getHistoryIndex() - 1);
                } else {
                    currentActive.classList.remove("focus");
                    this.setState(KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION);
                }
                break;
            case window.VK_DOWN:
                if (currentIndex + 3 < focusArray.length && !focusArray[currentIndex + 3].classList.contains("hide")) {
                    this.setFocus(focusArray[currentIndex + 3], currentActive);
                } else {
                    if (Sections.getHistoryIndex() + 9 < Sections.getHistoryLength()) {
                        Sections.renderHistory(Sections.getHistoryIndex() + 1);
                    }
                }
                break;
            case window.VK_LEFT:
                if (currentIndex % 3 == 0) {
                    currentActive.classList.remove("focus");
                    this.setState(KeyboardHandler.states.HISTORY_DATE_NAVIGATION);
                } else {
                    this.setFocus(focusArray[currentIndex - 1], currentActive);
                }
                break;
            case window.VK_RIGHT:
                if (currentIndex + 1 < focusArray.length && !focusArray[currentIndex + 1].classList.contains("hide")) {
                    this.setFocus(focusArray[currentIndex + 1], currentActive);
                } else if (currentIndex + 1 == focusArray.length) {
                    if (Sections.getHistoryIndex() + 9 < Sections.getHistoryLength()) {
                        Sections.renderHistory(Sections.getHistoryIndex() + 1);
                        this.setFocus(focusArray[focusArray.length - 3], currentActive);
                    } else {
                        Sections.renderHistory(0);
                        this.setFocus(focusArray[0], currentActive);
                    }
                }
                break;
            case window.VK_ENTER:
                if (currentActive.classList.contains("history_item")) {
                    currentActive.classList.remove("focus");
                    NativeApi.loadUrl(currentActive.dataset.url);
                    this.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
                } else if (currentActive.classList.contains("add_history_to_bookmark")) {
                    document.querySelector("#add-to-speed-dial-title").value = currentActive.dataset.title;
                    document.querySelector("#add-to-speed-dial-address").value = currentActive.dataset.url;
                    Popups.open("add-to-speed-dial");
                } else if (currentActive.classList.contains("del_history")) {
                    NativeApi.deleteHistory(currentActive.dataset.url, currentActive.dataset.time, function(result){
                        if (result[0].result == 0) {
                            var showItemsSize = document.querySelectorAll("#history_detail li").length * 3 - document.querySelectorAll("#history_detail .hide").length;
                            console.log("showItemsSize = " + showItemsSize + "; currentIndex = " + currentIndex);
                            if (currentIndex >= 2 && showItemsSize > 3) {
                                if(Sections._HistorystartIndex == 0 && currentIndex == showItemsSize - 1){
                                    console.log("Focus previous item");
                                    KeyboardHandler.setFocus(focusArray[currentIndex - 3], currentActive);
                                }else{
                                    console.log("Focus current item");
                                }
                            } else if (Sections.getHistoryIndex() > 0) {
                                Sections.renderHistory(Sections.getHistoryIndex() - 1);
                            } else {
                                currentActive.classList.remove("focus");
                                KeyboardHandler.setState(KeyboardHandler.states.HISTORY_CLEAR_ALL_NAVIGATION);
                            }
                            if(Sections._HistorystartIndex > 0){
                                Sections.renderHistory(Sections._HistorystartIndex - 1, Sections._StartTime, Sections._EndTime);
                }else{
                    Sections.renderHistory(Sections._HistorystartIndex, Sections._StartTime, Sections._EndTime);
                }
                        } else {
                            //TODO:
                            new Notification(i18n("There was a problem while clearing history"));
                        }
                    })
                }
                break;
            default:
                break;
        }
    },

    handleKeySpeedDial: function handleKeySpeedDial(e) {
        var focused = document.querySelector(".speeddial .focus");
        var currentLine = focused.parentNode;
        var currentLineElements = Array.prototype.slice.call(currentLine.querySelectorAll("li:not(.hide)"));
        var indexInLine = currentLineElements.indexOf(focused);
        var nextLine;
        if(currentLine.getAttribute("data-start-index")!="24"){
            nextLine= currentLine.nextElementSibling;
        }
        var prevLine = currentLine.previousElementSibling;
        var moveState = SpeedDial.isInMoveState();
        var curPageIndex;
        var pageNum = Math.ceil((SpeedDial.getList().length +1)/ 12);
        if (nextLine && !prevLine) {
            curPageIndex = 0;
        } else if (nextLine && prevLine) {
            curPageIndex = 1;
        } else if (!nextLine && prevLine) {
            if (pageNum == 3||pageNum==4) {
                curPageIndex = 2;
            } else if (pageNum == 2) {
                curPageIndex = 1;
            }
        }

        switch (e.keyCode) {
            case window.VK_LEFT:
                if (indexInLine == 0) {
                    if (prevLine) {
                        //More than 12 speedials
                        ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex - 1);
                        this.setFocusOrSwap(prevLine.querySelectorAll("li")[indexInLine + 11], focused, moveState);
                    }
                } else {
                    if (currentLineElements[indexInLine - 1]) {
                        speeddialNewfocus = currentLineElements[indexInLine - 1]
                        this.setFocusOrSwap(speeddialNewfocus, focused, moveState);
                    }
                }
                break;
            case window.VK_RIGHT:
                if (currentLineElements[indexInLine + 1]) {
                    speeddialNewfocus = currentLineElements[indexInLine + 1];
                    this.setFocusOrSwap(speeddialNewfocus, focused, moveState);
                } else if (nextLine) {
                    //More than 12 speedials
                    var nextLineElements = Array.prototype.slice.call(nextLine.querySelectorAll("li:not(.hide)"));

                    if (!moveState || nextLineElements[0].getAttribute("data-url")) {
                        if (nextLineElements[indexInLine - 11]) {
                            speeddialNewfocus = nextLineElements[indexInLine - 11];
                            ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex+1);
                            this.setFocusOrSwap(speeddialNewfocus, focused, moveState);
                        } else {
                            speeddialNewfocus = nextLineElements[0];
                            ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex+1);
                            this.setFocusOrSwap(speeddialNewfocus, focused, moveState);
                        }
                    }
                }
                break;
            case window.VK_UP:
                if (indexInLine < 4) {
                    if (prevLine) {
                        speeddialNewfocus = prevLine.querySelectorAll("li")[indexInLine + 8];
                        ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex-1);
                        //this.setFocusOrSwap(prevLine.querySelectorAll("li")[indexInLine + 8], cused, moveState);
                    } else {
                        focused.classList.remove("focus");
                        KeyboardHandler.setState(KeyboardHandler.states.BOOKMARKS_EDIT_NAVIGATION);
                    }
                } else {
                    speeddialNewfocus = currentLineElements[indexInLine - 4];
                    this.setFocusOrSwap(speeddialNewfocus, focused, moveState);
                }
                break;
            case window.VK_DOWN:
                if (indexInLine < 8 && currentLineElements.length > 4) {
                    if (currentLineElements[indexInLine + 4]) {
                        speeddialNewfocus = currentLineElements[indexInLine + 4];
                        this.setFocusOrSwap(currentLineElements[indexInLine + 4], focused, moveState);
                    } else if (currentLineElements[indexInLine + 1]) {
                        //this.setFocusOrSwap(currentLineElements[currentLineElements.length - 1], focused, moveState);
                        this.setFocusOrSwap(currentLineElements[indexInLine + 1], focused, moveState);
                        speeddialNewfocus = currentLineElements[indexInLine + 1]
                    }
                } else {
                    if (nextLine) {
                        var nextLineElements = Array.prototype.slice.call(nextLine.querySelectorAll("li:not(.hide)"));

                        if (!moveState || nextLineElements[0].getAttribute("data-url")) {
                            if (nextLineElements[indexInLine - 8]) {
                                speeddialNewfocus = nextLineElements[indexInLine - 8];
                                ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex+1);
                                this.setFocusOrSwap(nextLineElements[indexInLine - 8], focused, moveState);
                            } else {
                                speeddialNewfocus = nextLineElements[0];
                                ScrollBarManager.speedDialScrollBar.moveScrollBar(curPageIndex+1);
                                this.setFocusOrSwap(nextLineElements[0], focused, moveState);
                            }
                        }
                    }
                }
                break;
            case window.VK_ENTER:
                focused.click();
                break;
            case window.VK_ESCAPE:
                if (SpeedDial.isInEditState()) {
                    SpeedDial.disableEditing();
                }
                break;
            case window.VK_BACK:
                if(Application.backButton.classList.contains("disabled")){
                    sendCommand("exit_browser");
                } else {
                    Application.backButton.click();
                }
                break;
            case window.VK_BLUE:
                this.setFocus(speeddialNewfocus, 0);
                break;
        }
    },
    showNavigation:function (){
        KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, document.querySelector("#url"));
        KeyboardHandler.setNavTimer();
        //NativeApi.hideMouseCursor();
        document.querySelector(".header").style.cssText += "-webkit-transition: all 1s;";
        document.querySelector(".header").classList.remove("anihide");
        document.querySelector("#status-bar").classList.remove("hide");
        NativeApi.UIVisible=true;
        document.querySelector("#navigation").classList.add("active");
        document.querySelector("#url").classList.add("focus");
        document.querySelector("#url").focus();
        document.querySelector("#url").select();
        KeyboardHandler.state = KeyboardHandler.states.MENU_NAVIGATION;
    },
    handleKeyVirtualMouse: function handleKeyVirtualMouse(e, multiplicaction, longpress) {
        var viewSize = Application.getWindowSize();
        var self = this;
        NativeApi.getCursorAndScroll(function(args){
            //args[0] mouse.x
            //args[1] mouse.y
            //args[2] scroll_x
            //args[3] scroll_y
            //args[4] max_scroll_x
            //args[5] max_scroll_y
            var mouse_x=args[0];
            var mouse_y=args[1];
            var scroll_x=args[2];
            var scroll_y=args[3];
            var max_scroll_x=args[4];
            var max_scroll_y=args[5];
            switch (e.keyCode) {
                case window.VK_UP:
                    if (mouse_y > SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN ) {
                       // NativeApi.moveCursor(mouse_x, mouse_y - SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE * multiplicaction);
                    } else {
                        var focusItem = document.elementFromPoint(mouse_x, 50);
                        if (focusItem != null) {
                            if (focusItem.nodeName == "BUTTON" || focusItem.nodeName == "INPUT") {
                                KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, focusItem);
                            } else {
                                KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, document.querySelector("#url"));
                            }
                        } else {
                            KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, document.querySelector("#url"));
                        }
                        KeyboardHandler.setNavTimer();
                    }
                    break;
                case window.VK_DOWN:
                    if ((mouse_y + SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN > viewSize.height) && (scroll_y < max_scroll_y)) {
                        NativeApi.scroll(scroll_x, scroll_y + SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE* multiplicaction);
                    } else {
                        NativeApi.moveCursor(mouse_x, mouse_y + SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE * multiplicaction);
                    }
                    break;
                case window.VK_LEFT:
                    if (mouse_x >= SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN || scroll_x === 0) {
                        NativeApi.moveCursor(mouse_x - SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE * multiplicaction, mouse_y);
                    } else {
                        NativeApi.scroll(scroll_x - SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE* multiplicaction, scroll_y);
                    }
                    break;
                case window.VK_RIGHT:
                    if ((mouse_x + SETTINGS.VIRTUAL_MOUSE_SCROLL_WHEN_LESS_THAN > viewSize.width) && (scroll_x < max_scroll_x)) {
                        NativeApi.scroll(scroll_x + SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE* multiplicaction, scroll_y);
                    } else {
                        NativeApi.moveCursor(mouse_x + SETTINGS.VIRTUAL_MOUSE_SINGLE_MOVE_STEP_DISTANCE * multiplicaction, mouse_y);
                    }
                    break;
                case window.VK_ENTER:
                    NativeApi.mouseClick(mouse_x, mouse_y, 0);
                    break;
                case window.VK_BACK:
                case window.VK_ESCAPE:
                    if(Application.backButton.classList.contains("disabled")){
                        //Application.speedDialButton.click();
                        sendCommand("exit_browser");
                    }else{
                        document.getElementById("back").click();
                    }
                    break;
            }
        });
    },

    handleKeyPageNavigation: function handleKeyPageNavigation(e) {
        NativeApi.getScroll(function(scrollPos) {
            var x = parseInt(scrollPos[0], 10);
            var y = parseInt(scrollPos[1], 10);
            switch (e.keyCode) {
                case window.VK_UP:
                    if (y !== 0) {
                        NativeApi.scroll(0, y - 50);
                    } else {
                        KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION);
                    }
                    break;
                case window.VK_DOWN:
                    NativeApi.scroll(0, y + 50);
                    break;
                case window.VK_LEFT:
                    NativeApi.scroll(x - 50, 0);
                    break;
                case window.VK_RIGHT:
                    NativeApi.scroll(x + 50, 0);
                    break;
                case window.VK_BACK:
                case window.VK_ESCAPE:
                    document.getElementById("back").click();
                    break;
            }
        });
    },

    _getLeftPositionInside: function(el, untilElement) { // For scrolling...
        var left = 0;
        if (!untilElement) untilElement = document.body;
        while (el != untilElement) {
            left += +el.offsetLeft;
            el = el.parentNode;
        }
        return left
    },
    _findULInParent: function(el) {
        while (el.parentNode && el.nodeName != "UL") {
            el = el.parentNode;
        }
        return el;

    },

    setFocus: function(newFocus, oldFocus) {
        if (oldFocus) {
            oldFocus.classList.remove("focus");
            if (oldFocus.classList.contains("bookmark")) {
                oldFocus.classList.remove("focus_editing");
            }
        }

        {//add to blur focus item
            var allFocusItem = document.querySelectorAll(".focus");
            var index = 0;
            while (index < allFocusItem.length) {
                if ((document.defaultView.getComputedStyle(allFocusItem[index]).display != "none") &&
                    (allFocusItem[index].getBoundingClientRect().top >= 1080) &&
                    (allFocusItem[index].getBoundingClientRect().bottom >= 0) &&
                    (allFocusItem[index].children.length == 0)) {
                    try {
                        allFocusItem[index].classList.remove("focus");
                    } catch (e) {

                    }
                }
                index++;
            }

            index=0;
            var parent;
            if(newFocus.nodeName=="INPUT"){
                parent=newFocus.parentNode.parentNode;
            }else if(newFocus.nodeName=="LI"){
                parent=newFocus.parentNode.parentNode;
            }else{
                parent=newFocus.parentNode;
            }
            var focusedArray=parent.querySelectorAll(".focus");
            while(index<focusedArray.length){
                try{
                    focusedArray[index].classList.remove("focus");
                    if(focusedArray[index].nodeName=="INPUT"){
                        focusedArray[index].blur();
                    }
                }catch(e){
                    console.log(e);
                }
                index++;
            }
        }
        newFocus.classList.add("focus");
        if (oldFocus && oldFocus.nodeName == "INPUT") {
            oldFocus.classList.remove("focus");
            oldFocus.blur();
        }
        if (newFocus.nodeName == "INPUT") {
            newFocus.focus();
        }
        newFocus.dispatchEvent(new CustomEvent("customfocus"));

        // Only for seting focus in tabs where scroll is applicable
        if (!findParentContainingClass("active", newFocus)) return; // This check is for when mouse focus inactive line
        if (document.querySelector(".line.active")) {
            this.updateFocusInScrollableArea(newFocus);
        }
        if (document.querySelector(".speeddial.active")) {
            this.updateFocusInSpeedDialScrollableArea(newFocus);
        }
        if (document.querySelector(".done_bookmarks") && document.querySelector(".done_bookmarks").classList.contains("hide") !== true && newFocus.classList.contains("bookmark")) {
            newFocus.classList.add("focus_editing");
        }
    },
    updateFocusInScrollableArea: function(newFocus) {
        var parent = this._findULInParent(newFocus);
        if (parent && parent.style) {
            var offset = this._getLeftPositionInside(newFocus, parent);
            var oldValue = parseInt(parent.style && parent.style.WebkitTransform.replace("translate3d(", ""), 10) || 0;
            /*
            if (i18n.isRTL()) {
                if (offset + oldValue < 0) {
                    parent.style.WebkitTransform = "translate3d(" + Math.min(parent.scrollWidth - document.body.clientWidth + 400, -(newFocus.offsetLeft - parent.offsetWidth + 150)) + "px, 0, 0)";
                }
                if (offset + newFocus.offsetWidth + oldValue + 300 > document.body.clientWidth) {
                    parent.style.WebkitTransform = "translate3d(" + Math.max(75, -newFocus.offsetLeft) + "px, 0, 0)";
                }
            } else {

                if (offset + newFocus.offsetWidth + oldValue + 300 > document.body.clientWidth) {
                    parent.style.WebkitTransform = "translate3d(" + Math.max(-(parent.scrollWidth - document.body.clientWidth + 300), -newFocus.offsetLeft) + "px, 0, 0)";
                }
                if (offset + oldValue < 0) {
                    parent.style.WebkitTransform = "translate3d(" + Math.min(0, -newFocus.offsetLeft + document.body.clientWidth - newFocus.offsetWidth - 300) + "px, 0, 0)";
                }
            }
            */
        }
    },
    updateFocusInSpeedDialScrollableArea: function(newFocus) {
        if(findParentContainingClass("speeddials", newFocus)){
            var container = document.querySelector(".speeddials");
            //  container.style.WebkitTransform = "translate3d(-" + newFocus.parentNode.offsetLeft + "px,0,0)";
            container.style.WebkitTransform = "translate3d(0,-" + newFocus.parentNode.offsetTop + "px,0)";
        }
    },
    setFocusOrSwap: function(newFocus, oldFocus, swap) {
        if (!swap) {
            this.setFocus(newFocus, oldFocus);
        } else {
            // only swap items with data-url (only speeddials)
            if (newFocus.getAttribute("data-url") && oldFocus.getAttribute("data-url")) {
                var oldParent = oldFocus.parentNode;
                var newParent = newFocus.parentNode;
                var oldIndex = Array.prototype.slice.call(oldParent.children).indexOf(oldFocus);
                var newIndex = Array.prototype.slice.call(newParent.children).indexOf(newFocus);

                oldParent.removeChild(oldFocus);

                if (oldParent.nextElementSibling) {
                    oldParent.appendChild(oldParent.nextElementSibling.firstChild);
                }

                newParent.insertBefore(oldFocus, newParent.children[newIndex]);

                if (newParent.children.length > 12) {
                    newParent.nextElementSibling.insertBefore(newParent.children[12], newParent.nextElementSibling.firstChild);
                }
                if (oldParent.children.length > 12) {
                    oldParent.nextElementSibling.insertBefore(oldParent.children[12], oldParent.nextElementSibling.firstChild);
                }

                this.setFocus(oldFocus); // makes sure that proper view scrolling is applied
            }
        }
    },

    handleKeyMenu: function(e) {
        this.setNavTimer();
        var lines = Array.apply(null, document.querySelectorAll(".line"));
        var currentLine = document.querySelector(".line.active");
        if (!currentLine) return;
        var lineIndex = lines.indexOf(currentLine);
        var currentLineItems = Array.apply(null, currentLine.querySelectorAll("[tabindex]:not(.disabled)"));
        var currentLineActiveItem = currentLine.querySelector(".focus");
        var currentIndex = currentLineItems.indexOf(currentLineActiveItem);
        var editing = document.activeElement.nodeName == "INPUT";
        var allowWrap = currentLine.classList.contains("wrap");

        if (!currentLineActiveItem) {
            currentLineItems[0] && KeyboardHandler.setFocus(currentLineItems[0]);
            currentLineActiveItem = currentLine.querySelector(".focus");
        }
        var multiplicaction=1.5;
        if(e.repeat==true){
            multiplicaction=4.5;
        }
        switch (e.keyCode) {
            case window.VK_LEFT:
                if (editing) {
//                  return;
                    document.activeElement.blur();
                }
                if (currentIndex > 0) {
                    this.setFocus(currentLineItems[currentIndex - 1], currentLineActiveItem);
                    if (currentLineItems[currentIndex - 1].nodeName == "INPUT") {
                        currentLineItems[currentIndex - 1].select();
                    }
                } else if (allowWrap) {
                    this.setFocus(currentLineItems[currentLineItems.length - 1], currentLineActiveItem);
                    if (currentLineItems[currentLineItems.length - 1].nodeName == "INPUT") {
                        currentLineItems[currentLineItems.length - 1].select();
                    }
                }

                e.preventDefault();
                break;
            case window.VK_RIGHT:
                if (editing) {
//                  document.activeElement.blur();
//                  return;
                }
                if (currentIndex < currentLineItems.length - 1) {
                    this.setFocus(currentLineItems[currentIndex + 1], currentLineActiveItem);
                    if (currentLineItems[currentIndex + 1].nodeName == "INPUT") {
                        currentLineItems[currentIndex + 1].select();
                    }
                } else if (allowWrap) {
                    this.setFocus(currentLineItems[0], currentLineActiveItem);
                    if (currentLineItems[0].nodeName == "INPUT") {
                        currentLineItems[0].select();
                    }
                }

                e.preventDefault();
                break;
            case window.VK_DOWN:
            	this.setState(KeyboardHandler.states.TAB_NAVIGATION);
            	currentLineActiveItem.classList.remove("focus");
                    /*if (document.querySelector(".speeddial").classList.contains("show")) {
                        this.setState(KeyboardHandler.states.BOOKMARKS_HISTORY_NAVIGATION);
                    } else {
                        this.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION)
                    e.preventDefault();
                    if (editing) {
                        document.activeElement.blur();
                    }
                }*/
                break;
            case window.VK_UP:
                NativeApi.getScroll(function(scrollPos) {
                    var scroll_x = scrollPos[0];
                    var scroll_y = scrollPos[1];
                    var max_scroll_x = scrollPos[2];
                    var max_scroll_y = scrollPos[3];
                    //var mouse = MousePosition.get();
                    if(scroll_y!=0){
                        if(scroll_y >= SETTINGS.VIRTUAL_MOUSE_SCROLL_DISTANCE* multiplicaction ){
                            NativeApi.scroll(scroll_x, scroll_y - 300);
                        } else {
                            NativeApi.scroll(scroll_x, 0);
                        }
                    }
                });
//              KeyboardHandler.handleKeyVirtualMouse(e, 1, false);
                e.preventDefault();
                break;
            case window.VK_ENTER:
                if (editing) {
                    var _url = Application.urlInput.value;
                    if(_url != "") {
                        if (_url == "hisense://debug") {
                            NativeApi.loadUrl(HISENSE_DEBUG_URL);
                            Application.urlInput.blur();
                        } else {
                            Application.urlInput.blur();
                            Application.urlInput.focus();
                            /*
                            Bookmarks.add(_url, _url);
                            NativeApi.loadUrl(_url);
                            Application.urlInput.blur();
                            */
                        }

                    }
            else{
                        currentLineActiveItem.click();
                    }
                } else {
                    currentLineActiveItem.click();
                }

                e.stopPropagation();
                e.preventDefault();
                break;
            case window.VK_ESCAPE:
                if (editing) {
                    return;
                }
                if (document.querySelector(".speeddial").classList.contains("show")) {
                    this.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
                } else {
                    this.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
                }
                break;
            case window.VK_BACK:
                if(Application.backButton.classList.contains("disabled")){
                    sendCommand("exit_browser");
                }else{
                    Application.backButton.click();
                }
                break;
        }
    },
    _isElementVisibleInScrollableContent: function(element, container) {
        return (element.offsetTop >= container.scrollTop && (element.offsetTop + element.offsetHeight) <= (container.offsetHeight + container.scrollTop));

    },
    _moveScrollableContentOrSelectLink: function(container, moveUp) {
        var links = Array.apply(null, container.querySelectorAll("a"));
        var focused = container.querySelector(".focus");
        var index = links.indexOf(focused);

        if (moveUp) {
            if (index > 0 && this._isElementVisibleInScrollableContent(links[index - 1], container)) {
                KeyboardHandler.setFocus(links[index - 1], focused);
            } else {
                container.scrollTop -= 50;
            }
        } else {
            if (index < links.length - 1 && this._isElementVisibleInScrollableContent(links[index + 1], container)) {
                KeyboardHandler.setFocus(links[index + 1], focused);
            } else {
                container.scrollTop += 50;
            }
        }
    },

    handleKeyPopup: function(e) {
        var currentPopupElement = document.querySelector(".popup.active");
        var current = currentPopupElement.querySelector(".focus");
        var leftRight = current ? Array.apply(null, currentPopupElement.querySelectorAll("[tabindex=\"" + current.getAttribute("tabindex") + "\"]:not(.hide)")) : [];
        var leftRightIndex = leftRight.indexOf(current);
        var upDownIndex = current ? +current.getAttribute("tabindex") : 0;
        var firstItem = current ? currentPopupElement.querySelector("[tabindex]") : null;
        var lastItem = current ? Array.apply(null, currentPopupElement.querySelectorAll("[tabindex]")).pop() : null;
        var lastItemIndex = lastItem ? lastItem.getAttribute("tabindex") : -1;
        var editing = (document.activeElement.nodeName == "INPUT")&&(document.activeElement.id!="url");
        var scrollableContent = currentPopupElement.querySelector(".scrollable-content");
        var isScrollablePopupContent = !!scrollableContent;
        var toFocus;
        switch (e.keyCode) {
            case VK_BACK:
            case window.VK_ESCAPE:
                e.preventDefault();
                e.stopPropagation();
                Popups.close();
                break;
            case VK_LEFT:
                e.preventDefault();
                e.stopPropagation();
                if (editing || isScrollablePopupContent) {
                    if(current.selectionStart==0){
                        e.preventDefault();
                    }
                    return;
                }
                if (current == firstItem) {
                    KeyboardHandler.setFocus(lastItem, current);
                    return;
                }
                if (leftRightIndex == 0) {
                    if (document.querySelector(".popup.active").classList.contains("left-button-can-close")) {
                        Popups.close();
                    }
                    var nextUpDownIndex = upDownIndex - 1;
                    var nextLeftRight = Array.apply(null, currentPopupElement.querySelectorAll("[tabindex=\"" + nextUpDownIndex + "\"]:not(.hide)"));
                    if (nextLeftRight) {
                        KeyboardHandler.setFocus(nextLeftRight[nextLeftRight.length - 1], current);
                    }
                } else {
                    if (current && leftRight[leftRightIndex - 1]) {
                        KeyboardHandler.setFocus(leftRight[leftRightIndex - 1], current);
                    }
                }
                break;
            case VK_RIGHT:
                e.preventDefault();
                e.stopPropagation();
//                if (editing || isScrollablePopupContent) {
//                    if(current.value.length==current.selectionStart){
//                        e.preventDefault();
//                    }
//                    return;
//                }
                if (current == lastItem) {
                    KeyboardHandler.setFocus(firstItem, current);
                    return;
                }
                if (leftRightIndex < leftRight.length - 1) {
                    KeyboardHandler.setFocus(leftRight[leftRightIndex + 1], current);
                } else if (leftRightIndex == leftRight.length - 1) {
                    var nextUpDownIndex = upDownIndex + 1;
                    var nextLeftRight = Array.apply(null, currentPopupElement.querySelectorAll("[tabindex=\"" + nextUpDownIndex + "\"]:not(.hide)"));
                    if (nextLeftRight) {
                        KeyboardHandler.setFocus(nextLeftRight[0], current);
                    }
                }
                if (current && current.classList.contains("button-right-arrow")) {
                    current.click();
                }
                break;
            case VK_UP:
                e.preventDefault();
                e.stopPropagation();

                //Circulating movement
                if (upDownIndex == firstItem.getAttribute("tabindex")) {
                    KeyboardHandler.setFocus(lastItem, current);
                    return;
                }
                if (isScrollablePopupContent) {
                    this._moveScrollableContentOrSelectLink(scrollableContent, true);
                } else {
                    while (!toFocus && upDownIndex > 0) {
                        toFocus = currentPopupElement.querySelectorAll("[tabindex=\"" + (--upDownIndex) + "\"]:not(.hide)");
                    }
                    if (toFocus) {
                        var newFocus;
                        if (leftRightIndex + 1 > toFocus.length) {
                            newFocus = toFocus[toFocus.length - 1];
                        } else {
                            newFocus = toFocus[leftRightIndex];
                        }
                        KeyboardHandler.setFocus(newFocus, current);
                    }
                }
//              document.activeElement.blur();
                break;
            case VK_DOWN:
                e.preventDefault();
                e.stopPropagation();
                if (upDownIndex == lastItem.getAttribute("tabindex")) {
                    if(firstItem != current)
                        KeyboardHandler.setFocus(firstItem, current);
                    return;
                }
                if (isScrollablePopupContent) {
                    this._moveScrollableContentOrSelectLink(scrollableContent, false);
                } else {
                    while (!toFocus && upDownIndex < lastItemIndex) {
                        toFocus = currentPopupElement.querySelectorAll("[tabindex=\"" + (++upDownIndex) + "\"]:not(.hide)");
                    }
                    if (toFocus) {
                        if (leftRightIndex + 1 > toFocus.length) {
                            KeyboardHandler.setFocus(toFocus[toFocus.length - 1], current);
                        } else {
                            KeyboardHandler.setFocus(toFocus[leftRightIndex], current);
                        }
                    }
                }
//              document.activeElement.blur();
                break;
            case VK_ENTER:
                e.preventDefault();
                e.stopPropagation();
                if (!current) {
                    Popups.close();
                    break;
                }
                if (current.nodeName == "INPUT") {
                    if (editing) {
                        document.activeElement.blur();
                        current.focus();
                    }
                } else {
                    current.click();
                }
                break;
            default :
                break;
        }

    }

};

var Tab = function(url, title) {
    this.initHistory();
    this._url = "";
    this.setUrl(url);
    this.setTitle(title);
}
Tab.prototype = {
    initHistory: function() {
        this.History = new History();
    },
    setSecureState: function(val) {
        this._secureState = val;
    },
    setEvSsl: function(val) {
        this._ev_ssl = val;
    },
    setUrl: function(val) {
        this._url = val || this._url;
        this.History.setCurrentEntry(this._url);
    },
    setTitle: function(val) {
        this._title = val || i18n("No Name");
    },
    setCertificate: function(val) {
        this._certificate = val;
    },
    setConnection: function(val) {
        this._connection = val;
    },
    getSecureState: function() {
        return this._secureState;
    },
    getEvSsl: function() {
        return this._ev_ssl;
    },
    getUrl: function() {
        return this._url;
    },
    getTitle: function() {
        return this._title == "about:blank" ? i18n("Speed Dial") : this._title;
    },
    getCertificate: function(val) {
        return this._certificate;
    },
    getConnection: function(val) {
        return this._connection;
    },
    setZoom: function(val) {
        this._zoom = val;
    },
    getZoom: function() {
        return this._zoom;
    }
}

var Tabs = {
    _tabs: {},
    _activeTab: null,
    _count: 0,
    getActiveTab: function() {
        return this._tabs[this._activeTab];
    },
    getActiveTabId: function() {
        return this._activeTab;
    },
    setActiveTab: function(val) {
        this._activeTab = val;
        TabsLine.setActiveTab(this._activeTab);
    },
    addTab: function(id, url) {
        //if (typeof id === "undefined" || this._tabs[id]) return;
        this._tabs[id] = new Tab(this._count !== 0 ? "about:blank" : "", "");
        TabsLine.createNewtab(id,url);
        NativeApi.getZoom(id, function(res) {
            console.log("Current zoom level: " + res[0]);
            this._tabs[id].setZoom(res[0]);
        }.bind(this));
        this.updateTabUrl(id, url);
        this._count++;
        Sections.renderTabs();
        if (this._activeTab === null) {
            this.setActiveTab(id);
        }
    },
    removeTab: function(id) {
        //      delete this._tabs[id];
        Sections.removeTabById(id);
        this._count--;
        Sections.renderTabs();
        TabsLine.removeNewtab(id);
    },
    updateTabUrl: function(id, url) {
        if (!this._tabs[id]) {
            this.addTab(id, url);
        } else {
            this._tabs[id].setUrl(url);
        }
    },
    updateTabTitle: function(id, title) {
        this._tabs[id].setTitle(title);
        TabsLine.updateTabTitle(id,title);
        if (this._toNotify) {
            this._toNotify();
        }
    },
    addTitleNotifier: function(callback) {
        this._toNotify = callback;
    },
    getTab: function(id) {
        return this._tabs[id];
    },
    getAllTabs: function() {
        return this._tabs;
    },
    updateTabThumbnail: function(args) {
        var el = document.querySelector("#tabs-elements .tab[data-id='" + args[0] + "'] img");
        if (el) {
            el.src = args[3];
        }
    },
    getLength: function() {
        return this._count;
    }
}

var Sections = {
    init: function() {
        this.elementsContainer = document.querySelector("#elements");
        this.historyContainer = document.querySelector("#history-elements");
        this.tabsContainer = document.querySelector("#tabs-elements");
        //this.bookmarksContainer = document.querySelector("#bookmarks-elements");
        this.bookmarksContainer = document.getElementById("recommend");
        this.setupEvents();
        this._headerElement = document.querySelector(".header");
        this._speedDialElement = document.querySelector(".speeddial");
        KeyboardHandler.setFocus(document.querySelectorAll("#sections button")[1]);
        Tabs.addTitleNotifier(this._updateTitles);
        this._queueInterval = setInterval(this.queueForShowingThumbnails.bind(this), 3000);
        this.renderTabs();
        this.showTabs();
    },
    setupEvents: function() {
        var buttons = document.querySelectorAll("#sections button");
        for (var i = 0, l = buttons.length; i < l; i++) {
            buttons[i].addEventListener("customfocus", Sections.buttonEvent, false);
        }
        var that = this;
        this.elementsContainer.addEventListener("click", function(e) {
            that.handleButtonEvent(e);
        }, false);
    },
    handleButtonEvent: function(e) {
        if (e.target.nodeName != "LI" && e.target.parentNode.nodeName != "LI") {
            return;
        } // Cancel mouse clicks outside elements
        var element = findParentContainingClass("focus", e.target);
        if (element.classList.contains("new_tab")) {
            document.querySelector(".active").classList.remove("active");
            document.querySelector("#navigation").classList.add("active");
            KeyboardHandler.setFocus(document.querySelector("#url"), document.querySelector("#navigation .focus"));
            NativeApi.addTab("about:blank");
        }
        if (element.classList.contains("tab")) {
            if (this._editing) {
                NativeApi.removeTab(element.getAttribute("data-id"));
            } else {
                NativeApi.activateTab(element.getAttribute("data-id"));
                KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
            }
        }
        if (element.classList.contains("history_item")) {
            NativeApi.loadUrl(element.getAttribute("data-url"));
            KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
        }
        if (element.classList.contains("bookmark")) {
            if (this._editing) {
                if (this._editingState) {
                    var movedItem = document.querySelector(".bookmark.focus"); // Moving one is always in focus
                    var index = Array.prototype.slice.call(document.querySelectorAll(".bookmark")).indexOf(movedItem);
                    if (index > -1) {
                        Bookmarks.move(Bookmarks.getEntry(this._movingID).url, index);
                    }
                    this.setEditingState(false);
                } else {
                    Popups.open("manage-bookmark");
                }
            } else {
                NativeApi.loadUrl(Bookmarks.getEntry(+element.getAttribute("data-id")).url);
                KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
            }
        }
        if (element.classList.contains("close_tab")) {
            if (Tabs.getLength() == 1) {
                var replaceElement = document.querySelector(".tab[data-id=\"" + Tabs.getActiveTabId() + "\"]");
                replaceElement.classList.add("changing");
                setTimeout(function() {
                    NativeApi.removeTab(Tabs.getActiveTabId());
                }, 300);
            } else {
                NativeApi.removeTab(Tabs.getActiveTabId());
            }
        }
        if (element.classList.contains("clear_history")) {
            Popups.open("clear-history");
        }
        if (element.classList.contains("edit_bookmarks")) {
            this._editingBookmarks(true);
        }
        if (element.classList.contains("done_bookmarks")) {
            this.editingOff();
        }
        if (element.classList.contains("add_bookmark")) {
            Application.addToBookmarkView();
        }
    },
    removeTabById: function(id) {
        var element = document.querySelector(".tab[data-id=\"" + id + "\"]");
        if (element) {
            var list = Array.apply(null, element.parentNode.querySelectorAll("[tabindex]"));
            var index = list.indexOf(element);
            if (element.classList.contains("focus")) {
                KeyboardHandler.setFocus(list[index + 1], element);
            }
            KeyboardHandler.disableButton(element);
        }

    },
    _editingBookmarks: function(turnOn) {
        var done = document.querySelector(".done_bookmarks")
        var edit = document.querySelector(".edit_bookmarks");
        if (turnOn) {
            KeyboardHandler.enableButton(done);
            KeyboardHandler.disableButton(edit);
            KeyboardHandler.disableButton(document.querySelector(".add_bookmark"));
            KeyboardHandler.setFocus(done, edit);
            this._editing = true;

            var caption = document.querySelector("#sections_caption");

            document.querySelector("#sections_buttons").classList.add("hide");
            caption.classList.add("show");
            caption.innerText = i18n("Select a bookmark to edit");

            window.addEventListener("keydown", this._editingEvent, false);
        } else {
            KeyboardHandler.disableButton(done);
            KeyboardHandler.enableButton(edit);
            KeyboardHandler.enableButton(document.querySelector(".add_bookmark"));
            if (done.classList.contains("focus")) {
                KeyboardHandler.setFocus(edit);
            }
            this._editing = false;
        }
    },
    _editingEvent: function(e) {
        if ([VK_UP, VK_DOWN].indexOf(e.keyCode) > -1) {
            e.preventDefault();
            e.stopPropagation();
            Sections.editingOff();
        }
        if (!Sections._editingState) return;
        var currentFocus = document.querySelector(".line.active .focus");
        var currentMoving = document.querySelector(".bookmark[data-id=\"" + Sections._movingID + "\"]");
        if (e.keyCode == VK_LEFT || e.keyCode == VK_RIGHT) {
            if (currentMoving != currentFocus) {
                if (e.keyCode == VK_LEFT) {
                    currentMoving.parentNode.insertBefore(currentMoving, currentFocus);
                } else {
                    currentMoving.parentNode.insertBefore(currentMoving, currentFocus.nextSibling);
                }
                KeyboardHandler.setFocus(currentMoving, currentFocus);
            }
        }
    },
    editingOff: function() {
        Sections.setEditingState(false);
        Sections._editingBookmarks(false);
        document.querySelector("#sections_buttons").classList.remove("hide");
        document.querySelector("#sections_caption").classList.remove("show");
        window.removeEventListener("keydown", this._editingEvent, false);
        if (document.querySelector(".focus_editing")) {
            document.querySelector(".focus_editing").classList.remove("focus_editing");
        }
    },

    buttonEvent: function(e) {
        var cl = e.target.classList;
        if (cl.contains("tabs")) {
            Sections.showTabs();
        }
        if (cl.contains("history")) {
            Sections.showHistory();
        }
        if (cl.contains("bookmarks")) {
            Sections.showBookmarks();
        }
    },
    _updateTitles: function() {
        var tabs = Tabs.getAllTabs();
        for (var el in tabs)
            if (tabs.hasOwnProperty(el)) {
                document.querySelector("#tabs-elements [data-id=\"" + el + "\"] span").innerText = tabs[el].getTitle();
            }
    },
    _clearContainer: function(container) {
        container.innerHTML = "";
        container.style.WebkitTransform = "translate3d(0,0,0)";
    },
    renderTabs: function() {
        var tabs = Tabs.getAllTabs();
        var lastFocus = 0;
        this._clearContainer(this.tabsContainer);
        for (var el in tabs)
            if (tabs.hasOwnProperty(el)) {
                this.tabsContainer.innerHTML += "<li class='tab' data-last-focus='" + (lastFocus++) + "' data-id='" + el + "' tabindex='1'><span></span><img></li>";
                this.tabsContainer.lastChild.querySelector("span").innerText = tabs[el].getTitle();
                if (SETTINGS.SHOW_THUMBNAILS_IN_TABS) {
                    NativeApi.createThumbnail(+el, SETTINGS.THUMBNAIL_WIDTH, SETTINGS.THUMBNAIL_HEIGHT, Tabs.updateTabThumbnail);
                }
                if (tabs[el] == Tabs.getActiveTab()) {
                    this.tabsContainer.lastChild.classList.add("opened");
                }
            }
        this.tabsContainer.innerHTML += "<li class='new_tab' data-last-focus='100' tabindex='1' ><span>" + i18n("New Tab") + "</span></li>";
        this.tabsContainer.innerHTML += "<li class='close_tab' data-last-focus='101' tabindex='1'><span>" + i18n("Close Current Tab") + "</span></li>";
        var select = this.tabsContainer.querySelector(".tab[data-id=\"" + Tabs.getActiveTabId() + "\"]");
        if (select) {
            KeyboardHandler.setFocus(select)
        } else {
            KeyboardHandler.setFocus(this.tabsContainer.firstChild);
        }

    },
    renderBookmarks: function() {
        var bookmarks = Bookmarks.getList();
        if(bookmarks.length==0){
            document.querySelector("#recommended").style.visibility="hidden";
        }
        //      this._clearContainer(this.bookmarksContainer);
        document.getElementById("recommend").innerHTML = "";
        document.getElementById("recommend").innerHTML += "<ul></ul><ul></ul>";
        var bookmarksContainer = document.querySelectorAll(".recommend ul");
        var max_len = bookmarks.length > 6 ? 6 : bookmarks.length;
        for (var i = 0; i < max_len; i++) {
            bookmarksContainer[Math.floor(i / 3)].innerHTML += '<li class="recommend_item" data-url="' + Bookmarks.getEntry(i).url + '" data-title="' + Bookmarks.getEntry(i).title + '"><img class="recommend_img"><p class="recommend_label">' + Bookmarks.getEntry(i).title + '</p></li>'
            if(Bookmarks.getEntry(i).thumbnail != ""&&typeof(Bookmarks.getEntry(i).thumbnail) != "undefined"){
                var el = bookmarksContainer[Math.floor(i / 3)].querySelectorAll("li")[i % 3];
                el.querySelector("img").src = Bookmarks.getEntry(i).thumbnail;
                if(Bookmarks.getEntry(i).title==""){
                    el.dataset.title = Bookmarks.getEntry(i).url;
                    el.querySelector("p").innerHTML=Bookmarks.getEntry(i).url;
                }else{
                    el.dataset.title = Bookmarks.getEntry(i).title;
                    el.querySelector("p").innerHTML=Bookmarks.getEntry(i).title;
                }
                el.classList.add("thumbnail-handled");
            }
        }
        var bookmarksElements = document.querySelectorAll(".recommend li");
        for (var i = 0; i < max_len; i++) {
            bookmarksElements[i].setAttribute("tabindex", 1 + Math.floor(i / 3));
            bookmarksElements[i].addEventListener("click", function(e) {
                document.querySelector("#add-to-speed-dial-title").value = this.dataset["title"];
                document.querySelector("#add-to-speed-dial-address").value = this.dataset["url"];
                if(e.target.nodeName=="IMG"){
                    document.querySelector("#add-to-speed-dial-address").setAttribute("thumbNailStr",e.target.src);
                }else{
                    document.querySelector("#add-to-speed-dial-address").setAttribute("thumbNailStr",e.target.querySelector("img").src);
                }
            }, true);
        }
    },
    checkNoMoreBookmarks: function() {
        if (document.querySelectorAll("li.bookmark").length == 0) {
            var el = document.querySelector(".edit_bookmarks");
            if (el) {
                KeyboardHandler.disableButton(el);
            }
        }
    },
    getHistoryIndex: function() {
        return this._HistorystartIndex;
    },
    getHistoryLength: function() {
        return this._HistoryLength;
    },
    getCurrentDate: function(time) {
        var date = new Date(time);
        var monthArray = new Array(i18n("January"), i18n("February"), i18n("March"), i18n("April"),
                                   i18n("May"), i18n("June"), i18n("July"), i18n("August"),
                                   i18n("September"), i18n("October"), i18n("November"), i18n("December"));
        var weekArray = new Array(i18n("Sunday"), i18n("Monday"), i18n("Tuesday"),
                                  i18n("Wednesday"), i18n("Thursday"), i18n("Friday"), i18n("Saturday"));
        month = date.getMonth();
        day = date.getDate();
        if (day.toString().length == 1) {
            day = "0" + day.toString();
        }
        return monthArray[month] + " " + day + ", " + date.getFullYear() + ", " + weekArray[date.getDay()];
    },
    //Update history from starttime to endtime
    renderHistory: function(startIndex, starttime, endtime) {
        if (typeof(startIndex) == "undefined") {
            startIndex = 0;
        }
        this._HistorystartIndex = startIndex;
        if (typeof(endtime) == "undefined") {
            if (typeof(this._EndTime) == "undefined") {
                this._EndTime = new Date().setHours(23, 59, 59, 999);
            }
            endtime = this._EndTime;
        } else {
            this._EndTime = endtime;
        }

        if (typeof(starttime) == "undefined") {
            if (typeof(this._StartTime) == "undefined") {
                this._StartTime = new Date(Date.now()).setHours(0, 0, 0, 0);
            }
            starttime = this._StartTime;
        } else {
            this._StartTime = starttime;
        }
//      document.querySelector(".history_date").innerText = new Date(endtime).toDateString();
        document.querySelector(".history_date").innerText = this.getCurrentDate(endtime);
        var liLength = document.querySelectorAll("#history_detail li").length;
        NativeApi.getHistory(function(result) {
            if (result[1] != 0) {
                new Notification(i18n("Could not load history from memory"));
                return;
            }

            var _renderNum = 0;
            var _historyContainer = document.querySelector("#history_detail").children;
            var _li_index = 0;
            var history = result[0];

            try {
                if (history != null) {
                    var uniqueHistory = {},
                        uniqueHistoryArray = [];
                    for (var i = history.length - 1; i >= 0; i--) {
                        var entry = history[i];
                        if (entry.time > endtime || entry.time < starttime) {
                            continue;
                        }
                        if (!uniqueHistory[entry.url]) {
                            uniqueHistory[entry.url] = true;
                            uniqueHistoryArray.push(entry);
                            this._HistoryLength = uniqueHistoryArray.length;
                        }
                    }
                    //Scroll Bar contorl
                    var historyScrollBar = ScrollBarManager.historyScrollBar;
                    if(uniqueHistoryArray.length > liLength){
                        historyScrollBar.changeBarHeight(uniqueHistoryArray.length - liLength + 1);
                        historyScrollBar.show();
                        historyScrollBar.moveScrollBar(startIndex);
                    } else {
                        historyScrollBar.hide();
                    }

                    var _span;
                    _renderNum = uniqueHistoryArray.length - startIndex;
                    _renderNum = _renderNum > liLength ? liLength : _renderNum;

                    for (var el = startIndex; el < _renderNum + startIndex; el++) {
                        _historyContainer[_li_index].dataset.url = uniqueHistoryArray[el].url;
                        if (i18n.isRTL()){
                            _historyContainer[_li_index].innerHTML = "<span></span><img class='favicon'><span class='history_detail_url'></span><span></span>"
                        }else{
                            _historyContainer[_li_index].innerHTML = "<span></span><img class='favicon'><span></span><span class='history_detail_url'></span>"
                        }
                        _span = _historyContainer[_li_index].querySelectorAll("span");

                        _span[0].innerText = new Date(Number(uniqueHistoryArray[el].time)).Format("hh:mm:ss");
                        if (i18n.isRTL()){
                            _span[2].innerText = uniqueHistoryArray[el].title ? uniqueHistoryArray[el].title : uniqueHistoryArray[el].url;
                            _span[1].innerText = uniqueHistoryArray[el].url;
                        }else{
                            _span[1].innerText = uniqueHistoryArray[el].title ? uniqueHistoryArray[el].title : uniqueHistoryArray[el].url;
                            _span[2].innerText = uniqueHistoryArray[el].url;
                        }


                        var default_icon = "data:image/ico;base64,AAABAAEAEBAAAAEAIABoBAAAFgAAACgAAAAQAAAAIAAAAAEAIAAAAAAAAAQAANcNAADXDQAAAAAAAAAAAAB5eXkAQUFBEIKCgjuKiopAioqKQIuLi0CLi4tAi4uLQIqKikCCgoI8SUlJE7OzswAAAAAAAAAAAAAAAAAAAAAA19fXAIKCgjPj4+PW7+/v6vDw8Onx8fHp8vLy6fPz8+nt7e3p4ODg566urpdEREQd////AAAAAAAAAAAAAAAAAODg4ACJiYk47e3t6/r6+v/7+/v//f39//7+/v//////+fn6/+vr6//U1NT8s7Ozq05OTiT///8AAAAAAAAAAADg4OAAiYmJOO3t7ev6+vn/+/v7//z8/P/9/f3/7dS//+zWxP/w8PH/29vb/9/f3/6xsbGrQEBAHsPDwwAAAAAA39/fAIiIiDjs7Ozr+fn5//r6+v/7+/v//Pz8/+G1kf/kvp//9/f4/+jo6P/d3d3/2NjY/K6urpotLS0SX19fAN/f3wCIiIg47Ozr6/j4+P/6+vr/+/v7//z8/f/58+//+/by//z8/f/39/f/8vLy/+/v7//k5OTogYGBNdLS0gDf398AiIiIOOvr6+v39/f/+fn5//r6+v/8/f3/9Onf//Hg0f/+/////f39//v7+//6+vr/8PDw64qKijji4uIA3t7eAIiIiDjq6unr9vb2//j49//5+fn/+/v7//jy7f/jv6D/+vXw//7+/v/+/v7//v7+//Pz8+uLi4s44+PjAN7e3QCHh4c46eno6/X19P/29vb/+Pj5//r7/P/7/P3/4r6f/+G6mP/8/Pz//f39//39/f/y8vLri4uLOOPj4wDd3d0Ah4eHOOfn5+vz8/L/9fX1//Pv6//u4NP/+fr6/+/e0P/Lh0v/8uXZ//z9/f/8/Pz/8fHx64qKijji4uIA3NzcAIeHhjjm5uXr8fHx//T19f/kzbf/z5hl//b18//z6uL/x38//+nRu//7/P7/+/v7//Dw8OuKioo44uLiANzc2wCGhoY45eXk6+/v7//x8vL/6dzQ/9q1kv/v5t7/4L+h/8yNVf/x597/+fn6//n5+f/u7u7riYmJOOHh4QDb29oAhYWFOOPj4uvu7u3/7+/u//Dw8P/p3tL/4cev/96/ov/t4db/9vb2//b29v/39/b/7Ozs64mJiTjg4OAA2tnZAIWEhDjh4eDr7Ozr/+3t7P/u7u7/8PHx//Hy8v/z9PT/8/T0//Pz8//09PP/9PT0/+rq6uuIiIg439/eANTU0wCBgYEy2tnZ0+Tj4ufl5eTm5ubl5ufn5+bo6Ojm6enp5urq6ebq6urm6+vq5uvr6+fi4uHThISEMtjY2ACQkJAAUVFRDZiXlzGfn581oKCfNaCgoDWhoaA1oaGhNaGhoTWioqE1oqKhNaKiojWioqI1m5uaMVJSUg2RkZEAgB8AAIAPAACABwAAgAMAAIABAACAAQAAgAEAAIABAACAAQAAgAEAAIABAACAAQAAgAEAAIABAACAAQAAgAEAAA==";
                        var store_icon = uniqueHistoryArray[el].icon;

                        _historyContainer[_li_index].querySelector("img").src = store_icon ? store_icon : default_icon;
                        _historyContainer[_li_index++].classList.remove("hide");

                        _historyContainer[_li_index].dataset.url = uniqueHistoryArray[el].url;
                        _historyContainer[_li_index].dataset.title = uniqueHistoryArray[el].title;
                        _historyContainer[_li_index++].classList.remove("hide");

                        _historyContainer[_li_index].dataset.url = uniqueHistoryArray[el].url;
                        _historyContainer[_li_index].dataset.time = uniqueHistoryArray[el].time;
                        _historyContainer[_li_index++].classList.remove("hide");
                    }
                }
            } catch(e) {
                console.log(e);
            }
            for (var el = _renderNum + startIndex; el < liLength + startIndex; el++) {
                _historyContainer[_li_index++].classList.add("hide");
                _historyContainer[_li_index++].classList.add("hide");
                _historyContainer[_li_index++].classList.add("hide");
            }

        }.bind(this));
    },
    setState: function(state) {
        this._state = state;
    },
    setEditingState: function(state) {
        var caption = document.querySelector("#sections_caption");
        var done_bookmarks = this.elementsContainer.querySelector(".done_bookmarks");
        if (done_bookmarks) {
            KeyboardHandler.enableButton(done_bookmarks);
        }
        if (state) {
            if (done_bookmarks) {
                KeyboardHandler.disableButton(done_bookmarks);
            }
            this._movingID = document.querySelector(".bookmark.focus").getAttribute("data-id");
            caption.innerText = i18n("Select a new location for the bookmark");

        } else {
            caption.innerText = i18n("Select a bookmark to edit");
        }

        this._editingState = state;
    },
    getState: function() {
        return this._state;
    },
    hideOldContainer: function() {
        var old = this.elementsContainer.firstChild;
        if (old) {
            document.querySelector("#hidden-elements").appendChild(old);
        }
    },
    clearMarqueesOnElementsContainer: function() {
        var elements = this.elementsContainer.querySelectorAll(".marquee-animate");
        for (var i = elements.length - 1; i >= 0; i--) {
            elements[i].setAttribute("style", "");
            elements[i].classList.remove("marquee-animate");
        }
    },
    showTabs: function(stillEditing) {
        this._editing = stillEditing;
        this.setState("tabs");
        this.hideOldContainer();
        this.elementsContainer.appendChild(this.tabsContainer);
        this.clearMarqueesOnElementsContainer();
    },
    showHistory: function() {
        this.setState("history");
        this.hideOldContainer();
        this.elementsContainer.appendChild(this.historyContainer);
    },
    showBookmarks: function(stillEditing) {
        this._editing = stillEditing;
        this.setState("bookmarks");
        this.hideOldContainer();
        this.elementsContainer.appendChild(this.bookmarksContainer);
    },
    lastThumbnailUrl:null,
    repetition:0,
    queueForShowingThumbnails: function() {
        //Max 20s to waited
        if (Date.now() - this._queueMutex > 30000) {
            this._queueMutex = false;
        } // Make sure that mutex is cleared so it won't stuck in some edge case
        if (this._queueMutex) {
            return;
        }

        //TODO:when create thumbnail?
        if (this._speedDialElement.classList.contains("show")) {
            if (SETTINGS.SHOW_THUMBNAILS_IN_SPEED_DIAL){
                var el = document.querySelector(".speed-dial-entry:not(.thumbnail-handled)");
                if (el) {
                    var size = SpeedDial.thumbnailSize();
                    var url = el.getAttribute("data-url");
                    if(url==Sections.lastThumbnailUrl){
                        Sections.repetition++;
                    }else{
                        Sections.repetition=0;
                    }
                    console.log("Loading SpeedDial Thumbnail for: " + url);
                    this._queueMutex = Date.now();
                    if(Sections.repetition==2){
                        console.log("repetition=2, set default thumbnail for "+url);
                        el.querySelector("img").src=defaultImageSrc;
                        el.classList.add("thumbnail-handled");
                        Sections.repetition=0;
                    }else{
                        NativeApi.createThumbnailForUrl(url, size[0], size[1], SpeedDial.updateSpeedDialThumbnail.bind(el));
                    }
                    Sections.lastThumbnailUrl=url;
                }
            }
            if (SETTINGS.SHOW_THUMBNAILS_IN_BOOKMARKS){
                var recommend = document.querySelector(".recommend_item:not(.thumbnail-handled)");
                if (recommend) {
                    var url = recommend.getAttribute("data-url");
                    if(url==Sections.lastThumbnailUrl){
                        Sections.repetition++;
                    }else{
                        Sections.repetition=0;
                    }
                    console.log("Loading Bookmark Thumbnail for: " + url);
                    this._queueMutex = Date.now();
                    if(Sections.repetition==2){
                        console.log("repetition=2, set default thumbnail for "+url);
                        recommend.querySelector("img").src=defaultImageSrc;
                        recommend.classList.add("thumbnail-handled");
                        Sections.repetition=0;
                    }else{
                        NativeApi.createThumbnailForUrl(url, SETTINGS.THUMBNAIL_WIDTH, SETTINGS.THUMBNAIL_HEIGHT, Bookmarks.updateBookmarksThumbnail);
                    }
                    Sections.lastThumbnailUrl=url;
                }
            }
        }
    },
    unblockThumbnailMutex: function() {
        this._queueMutex = false;
        this.queueForShowingThumbnails(); // When image was loaded, call the function immediately to speedup getting images from cache
    }

}

/**
 * Singleton Loading holder
 */
var LoadingBar = {
    _loadingInterval: -1,
    _loadingRotate: 0,
    init: function() {
        this.el = document.querySelector("#progress");
        this.reloadStopButton = document.querySelector("#reloadStop");
    },
    progress: function(e) {
//      don't show this loading bar,use web window display the loading bar this just hide'
      //  NativeApi.show(); // Make sure we got progress visible during loading
        if (e.progress && e.progress > -1) {
            LoadingBar.el.style.WebkitTransform = "scale(" + (Math.max(0, Math.min(1, +e.progress))) + ",1)";
            if (LoadingBar.reloadStopButton.classList.contains("fa-refresh")) {
                LoadingBar.reloadStopButton.classList.remove("fa-refresh");
                LoadingBar.reloadStopButton.classList.add("fa-close");
            }
        }

        if (e.progress == 1 || e.progress > 0.95) {
            LoadingBar.hide();
        }

    },
    show: function() {
        //TODO:show and focus??????
        //NativeApi.show();
        NativeApi.gainFocus();

        LoadingBar.el.classList.add("show");
        document.querySelector(".header").classList.add("loading");
    },
    hide: function() {
        LoadingBar.el.style.WebkitTransform = "scale(0,0)";
        LoadingBar.el.classList.remove("show");
        document.querySelector(".header").classList.remove("loading");
        if (LoadingBar.reloadStopButton.classList.contains("fa-close")) {
            LoadingBar.reloadStopButton.classList.remove("fa-close");
            LoadingBar.reloadStopButton.classList.add("fa-refresh");
        }
        if(KeyboardHandler.getState()!=KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY){
            NativeApi.blurFocus();
        }
    },
    isShown: function() {
        return LoadingBar.el.classList.contains("show");
    }
};

var Bookmarks = {
    _list: [],
    getList: function() {
        return this._list;
    },
    getEntry: function(i) {
        return this._list[i];
    },
    add: function(url, title) {
        NativeApi.saveBookmark(url, title, function(result) {
            this.saveCallback(result, url, title);
        }.bind(this));
    },

    move: function(url, position) {
        NativeApi.moveBookmark(url, position, function(result) {
            this.moveCallback(result, position);
        }.bind(this));
    },
    remove: function(url, title) {
        NativeApi.deleteBookmark(url, title, this.removeCallback.bind(this));
    },
    findItemByUrl: function(url) {
        var index = -1;
        for (var i = 0; i < this._list.length; i++) {
            if (this._list[i].url == url) {
                index = i;
            }
        }
        return index;
    },

    saveCallback: function(result, url, title) {
        switch (result[1]) {
            case 0: // OK
                this._list.push({
                    url: SETTINGS.BOOKMARK_URL_LIMIT == -1 ? url : url.substr(0, SETTINGS.BOOKMARK_URL_LIMIT),
                    title: SETTINGS.BOOKMARK_TITLE_LIMIT == -1 ? title : title.substr(0, SETTINGS.BOOKMARK_TITLE_LIMIT)
                });
                if(this._list.length==7){
                    this._list.shift();
                }
                //new Notification(i18n("Bookmark saved"));
                console.log(i18n("Bookmark saved"));
                Sections.renderBookmarks();
                break;
            case 1: // could not save

                //new Notification(i18n("Could not save bookmark in memory"));
                console.log(i18n("Could not save bookmark in memory"));
                break;
            case 3:
                //new Notification(i18n("Bookmark not saved: you have reached the maximum number"));
                console.log(i18n("Bookmark not saved: you have reached the maximum number"));
                break;
            case 2: // bookmark is on list
                //new Notification(i18n("Bookmark is already on the list!"));
                console.log(i18n("Bookmark is already on the list!"));
                break;
            case 4:
                //new Notification(i18n("Bookmark not saved: the address is too long"));
                console.log(i18n("Bookmark not saved: the address is too long"));
                break;
        }
    },
    moveCallback: function(result, position) {
        switch (result[1]) {
            case 0: // OK
                var index = this.findItemByUrl(result[0]);
                if (index > -1) {
                    this._list.splice(position, 0, (this._list.splice(index, 1))[0]);
                }
                //new Notification(i18n("Bookmark moved"));
                console.log("Bookmark moved");
                break;
            case 1: // could not save
            case 3: // could not load
            case 5: // wrong positon
                //new Notification(i18n("Could not modify bookmark in memory"));
                console.log("Could not modify bookmark in memory");
                break;
            case 4: // bookmark no longer exist on list
                //new Notification(i18n("Bookmark not found in device memory"));
                console.log("Bookmark not found in device memory");
                break;
        }
        if (Sections.getState() == "bookmarks") {
            Sections.editingOff();
        }
        Sections.renderBookmarks();
    },
    removeCallback: function(result) {
        switch (result[1]) {
            case 0: // OK
                var index = this.findItemByUrl(result[0]);
                if (index > -1) {
                    this._list.splice(index, 1);
                    var el = document.querySelector(".bookmark[data-id=\"" + index + "\"");
                    if (el) {
                        el.parentNode.removeChild(el);
                        KeyboardHandler.setFocus(document.querySelector("#elements li[tabindex]"));
                    }

                }
                Sections.setEditingState(false);
                //new Notification(i18n("Bookmark deleted"));
                console.log("Bookmark deleted");
                break;
            case 1: // could not save
            case 3: // could not load
                //new Notification(i18n("Could not delete bookmark from memory"));
                console.log("Could not delete bookmark from memory");
                break;
            case 4: // bookmark no longer exist on list
                //new Notification(i18n("Bookmark not found in device memory"));
                console.log("Bookmark not found in device memory");
                break;
        }
        if (Sections.getState() == "bookmarks") {
            Sections.editingOff();
        }
        Sections.renderBookmarks();
    },


    init: function() {
        this.element = document.querySelector(".speeddial");
        this.container = this.element.querySelector(".speeddials");
        NativeApi.getAllBookmarks(this.onGetAllBookmarks.bind(this));
    },
    onGetAllBookmarks: function(obj) {
        if (obj[1] != 3) {
            this._list = [];
            for (var i = 0; i < obj[0].length; i++) {
                this._list.push({
                    title: obj[0][i].title,
                    url: obj[0][i].url,
                    thumbnail : obj[0][i].thumbnail
                });
            }
            Sections.renderBookmarks();

        } else {
            //new Notification(i18n("Could not load bookmarks file!"));
            console.log("Could not load bookmarks file!");
        }
        //add top history to bookmarks
        if(this._list.length<6){
            //var top=6-this._list.length;
            var top=6;
            NativeApi.getHistory(function(result) {
                var history = result[0];
                if (history != null) {
                    for (var i =  0; i < history.length; i++) {
                        var j=0;
                        var k=0;
                        for(j=0;j<Bookmarks._list.length;j++){
                            if(Bookmarks._list[j].url==history[i].url||Bookmarks._list[j].title==history[i].title){
                                break;
                            }
                        }
                        for(k=0;k<SpeedDial._list.length;k++){
                            if(SpeedDial._list[k].url==history[i].url||SpeedDial._list[k].title==history[i].title){
                                break;
                            }
                        }
                        if(j+k==Bookmarks._list.length+SpeedDial._list.length){
                            if(Bookmarks._list.length<6){
                                Bookmarks._list.push({
                                    title: history[i].title,
                                    url: history[i].url,
                                    thumbnail : ""
                                });
                            }else{
                                break;
                            }
                        }
                    }
                    Sections.renderBookmarks();
                }
            },top);
        }
    },
    updateBookmarksThumbnail: function(args) {
//      var el = document.querySelector(".bookmark[data-url='" + args[1] + "'] img");
        var parentItem = document.querySelector(".recommend_item[data-url='" + args[1] + "']");
        var img = parentItem.querySelector("img");
        if (img) {
            for(var i = 0;i < Bookmarks._list.length;i++){
                if(Bookmarks._list[i].url == args[1]){
                    if(args[5]=="default"){
                        Bookmarks._list[i].thumbnail = defaultImageSrc;
                        Bookmarks._list[i].title = args[1];
                    }else{
                        Bookmarks._list[i].thumbnail = args[5];
                        if (args[4]) {
                            Bookmarks._list[i].title = args[4];
                        }
                    }
                    break;
                }
            }
            img.src = args[5];
            if (args[5]) {
                parentItem.classList.add("thumbnail-handled");
            }
            if (args[4]) {
                parentItem.dataset.title = args[4];
            }
            for(var i=0;i<SpeedDial._list.length;i++){
                if(SpeedDial._list[i].url==args[1]){
                    if(args[5]=="default"){
                        SpeedDial._list[i].thumbnail = defaultImageSrc;
                    }else{
                        SpeedDial._list[i].thumbnail = args[5];
                    }
                }
            }
        }
        Sections.unblockThumbnailMutex();
        if(!document.querySelector(".speeddial").classList.contains("show")){
            console.log("destroyThumbnailWindow");
            NativeApi.destroyThumbnailWindow();
        }else{
            if((!document.querySelector(".speed-dial-entry:not(.thumbnail-handled)"))&&(!document.querySelector(".recommend_item:not(.thumbnail-handled)"))){
                console.log("destroyThumbnailWindow");
                NativeApi.destroyThumbnailWindow();
            }
        }
    }
};

var SpeedDial = {
    init: function() {
        this._list = [];
        this._editing = false;
        this.container = document.querySelector(".speeddials");
        this.container.addEventListener("click", this.handleButtonEvent.bind(this), false);

        this.waitForContainerSize(function() {
            var thumbnailSize = SpeedDial.thumbnailSize();
            NativeApi.changePreferences(true, thumbnailSize[0], thumbnailSize[1]);
            NativeApi.getSpeedDials(function(result) {
                if (result[1] == 0) {
                    if (!result[0]) {
                        result[0] = [];
                    }
                    this.setList(result[0]);
                } else {
                    new Notification(i18n("Could not load Speed Dial entries"));
                }
                this.render();
            }.bind(this));
        }.bind(this));
        document.getElementById("loading_status").classList.add("hide");
        KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
    },
    // In some rare cases container might not be ready to take measurements from,
    // this method will wait for proper size
    waitForContainerSize: function(callback) {
        if (SpeedDial.container.offsetWidth > 0) {
            callback();
        } else {
            setTimeout(function() {
                SpeedDial.waitForContainerSize(callback)
            }, 100);
        }
    },
    thumbnailSize: function(e) {
        //var width = SpeedDial.container.offsetWidth;
        //width is 373
        var oneTileSizeWithMarginIncluded = 373;//(width / 4) - 10; // 10px of margins
        return [oneTileSizeWithMarginIncluded, Math.floor(oneTileSizeWithMarginIncluded * 9 / 16)];
    },
    handleButtonEvent: function(e) {
        Popups.prefocused=null;
        var focused = findParentContainingClass("focus", e.target);
        if (!focused) {
            return;
        }
        if (focused.classList.contains("edit_speed_dial")) {
            this.enableEditing();
        } else if (focused.classList.contains("add_speed_dial")) {
            //document.querySelector("#add-to-speed-dial-title").value = "";
            document.querySelector("#add-to-speed-dial-address").value = "";
            Sections.renderBookmarks();
            Popups.prefocused=focused;
            Popups.open("add-to-speed-dial");
            //document.querySelector("#add-to-speed-dial-address").focus();
        } else if (focused.classList.contains("done_speed_dial")) {
            this.disableEditing();
        } else {
            if (this._editing) {
                if (this._moving) {
                    var el = document.querySelector(".speeddials .focus");
                    this.move(this._list[this._movingID].url, +el.parentNode.getAttribute("data-start-index") + Array.prototype.slice.call(el.parentNode.children).indexOf(el));
                    this.disableMoveState();
                } else {
                    var captionElement = document.querySelector("#speeddial-caption");
                    var previousText = captionElement.innerText;
                    captionElement.innerText = i18n("Select action");
                    /*
                    Popups.prefocused=focused;
                    Popups.open("manage-speed-dial", function() {
                        captionElement.innerText = previousText;
                    });
                    */
                    SpeedDial.remove(focused.getAttribute("data-url"));
                }
            } else {
                NativeApi.loadUrl(focused.getAttribute("data-url"));
            }
        }
    },
    enableEditing: function() {
        //      var focused = document.querySelector(".edit_speed_dial");
        //      focused.classList.remove("edit_speed_dial");
        //      focused.classList.add("done_speed_dial");
        this._editing = true;
        //document.querySelector(".speeddial").classList.add("show-caption");
        //document.querySelector("#speeddial-caption").innerText = i18n("Select a Speed Dial entry to edit");
        document.querySelector("#management_exit").innerText = i18n("Exit");
        var deletes = document.querySelectorAll(".delete");
        if (deletes.length>0) {
            var index=0;
            while(index<deletes.length){
                deletes[index].classList.remove("delete");
                deletes[index].classList.add("delete_show");
                index++;
            }
        }
    },
    disableEditing: function() {
        //      var focused = document.querySelector(".done_speed_dial");
        this._moving = false;
        //      if (focused) {
        //          focused.classList.add("edit_speed_dial");
        //          focused.classList.remove("done_speed_dial");
        this._editing = false;
        this.render();
        //      }
        document.querySelector(".speeddial").classList.remove("show-caption");
        document.querySelector("#management_exit").innerText = i18n("Management");
    },
    getList: function() {
        return this._list;
    },
    getEntry: function(i) {
        return this._list[i];
    },
    setList: function(list) {
        this._list = list;
    },
    findItemByUrl: function(url) {
        var index = -1;
        for (var i = 0; i < this._list.length; i++) {
            if (this._list[i].url == url) {
                index = i;
            }
        }
        return index;
    },
    add: function(url, title,thumbNailStr) {
        if(thumbNailStr==null){
            thumbNailStr="";
        }
        NativeApi.addSpeedDial(url, title,thumbNailStr, function(res) {
            this.addCallback(res, title,thumbNailStr);
        }.bind(this));
    },
    move: function(url, position) {
        NativeApi.moveSpeedDial(url, position, function(res) {
            this.moveCallback(res, position);
        }.bind(this));
    },
    remove: function(url) {
        NativeApi.removeSpeedDial(url, this.removeCallback.bind(this));
    },
    edit: function(url, newUrl, newTitle) {
        /*
        var index = this.findItemByUrl(url);
        if (index > -1) {
            NativeApi.removeSpeedDial(url, function(r) {
                NativeApi.addSpeedDial(newUrl, newTitle, function(r) {
                    NativeApi.moveSpeedDial(newUrl, index, function(r) {
                        this.editCallback(r, index, newUrl, newTitle);
                    }.bind(this));
                }.bind(this));
            }.bind(this));
        } else {
            this.editCallback();
        }
        */
        NativeApi.modifySpeedDial(url, newUrl, newTitle,function(result){
            if(result[0]==0){
                var index = SpeedDial.findItemByUrl(url);
                SpeedDial._list[index].url=newUrl;
                SpeedDial._list[index].title=newTitle;
                if(url!=newUrl){
                    //SpeedDial._list[index].classList.remove("thumbnail-handled");
                    SpeedDial._list[index].thumbnail="";
                }
                SpeedDial.render();
            }
        });
    },
    editCallback: function(result, index, url, title) {
        if (result && result[1] === 0) {
            new Notification(i18n("Speed Dial entry modified"));
            this._list[index] = {
                url: url,
                title: title
            };
            this.render();
        } else {
            new Notification(i18n("Could not modify Speed Dial entry"));
        }
    },
    addCallback: function(result, title,thumbNailStr) {
        switch (result[1]) {
            case 0:
                this._list.push({
                    url: SETTINGS.SPEEDDIAL_URL_LIMIT == -1 ? result[0] : result[0].substr(0, SETTINGS.SPEEDDIAL_URL_LIMIT),
                    title: SETTINGS.SPEEDDIAL_TITLE_LIMIT == -1 ? title : title.substr(0, SETTINGS.SPEEDDIAL_TITLE_LIMIT),
                    thumbnail: thumbNailStr
                });
                this.render();
                new Notification(i18n("Bookmark added"));
                break;
            case 1:
                new Notification(i18n("Add failed"));
                break;
            case 2:
                new Notification(i18n("Website already added"));
                break;
            case 3:
                new Notification(i18n("Speed Dial entry not saved: you have reached the maximum number"));
                break;
            case 4:
                new Notification(i18n("Speed Dial entry not saved: the address is too long"));
                break;
        }
    },
    moveCallback: function(result, position) {
        switch (result[1]) {
            case 0:
                var index = this.findItemByUrl(result[0]);
                if (index > -1) {
                    this._list.splice(position, 0, (this._list.splice(index, 1))[0]);
                }
                new Notification(i18n("Speed Dial entry moved"));
                this.render();
                break;
            case 1:
            case 3:
            case 4:
            case 5:
                new Notification(i18n("Could not move Speed Dial entry"));
                break;
        }
    },
    removeCallback: function(result) {
        switch (result[1]) {
            case 0:
                var index = this.findItemByUrl(result[0]);
                if (index > -1) {
                    this._list.splice(index, 1);
                }
                this.render();
                //new Notification(i18n("Speed Dial entry deleted"));
                break;
            case 1:
            case 3:
            case 4:
                //new Notification(i18n("Could not delete Speed Dial entry"));
                break;
        }
    },
    render: function() {
        var lastIndex = 0;
        if (this.container.innerHTML) {
            var currentFocus = this.container.querySelector(".focus");
            var selectableLiArray = Array.prototype.slice.call(this.container.querySelectorAll("li") || []);
            var lastIndex = selectableLiArray.indexOf(currentFocus);
        }
        this.container.innerHTML = "";
        this.container.style.WebkitTransform = "translate3d(0,-16px,0)";
        var ul, li;
        //      var list = this._list.concat({
        //          edit: true
        //      }, {
        //          add: true
        //      });

        var list = this._list.concat({
            add: true
        });

        var speedDialScrollBar = ScrollBarManager.speedDialScrollBar;
        if (list.length < 12) {
            speedDialScrollBar.hide();
        } else if (list.length >= 12 && list.length <= 23) {
            speedDialScrollBar.changeBarHeight(2);
            speedDialScrollBar.show();
            speedDialScrollBar.moveScrollBar(0);
        } else if (list.length >= 24) {
            speedDialScrollBar.changeBarHeight(3);
            speedDialScrollBar.show();
            speedDialScrollBar.moveScrollBar(0);
        }

        for (var i = 0; i < list.length; i++) {
            if (i % 12 == 0) {
                ul = this.container.appendChild(document.createElement("ul"));
                ul.setAttribute("data-start-index", i);
            }
            if(i==36){
                return;
            }
            li = ul.appendChild(document.createElement("li"));
            li.setAttribute("tabindex", 1);
            //TODO?
            //li.setAttribute("data-focus-group", "speeddial");
            li.setAttribute("data-state", "SPEED_DIAL_PAGE_NAVIGATION");
            if (list[i].url) {
                li.setAttribute("data-url", list[i].url);
                li.setAttribute("data-title", list[i].title);
                li.setAttribute("data-id", i);
                li.classList.add("speed-dial-entry");
                if(SpeedDial._editing){
                    li.innerHTML = "<img class='speedDial_img'><div class='delete_show fa fa-times-circle'></div><p class='speeddial_title'></p>";
                }else{
                    li.innerHTML = "<img class='speedDial_img'><div class='delete fa fa-times-circle'></div><p class='speeddial_title'></p>";
                }
                if (list[i].thumbnail != "") {
                    li.querySelector("img").src = list[i].thumbnail;
                    li.classList.add("thumbnail-handled");
                }
                if(list[i].title==""){
                    li.querySelector("p").innerText = list[i].url;
                }else{
                    li.querySelector("p").innerText = list[i].title;
                }
            } else {
                if (list[i].edit) {
                    li.className = "edit_speed_dial";
                    li.innerHTML = "<span class='edit'><img>" + i18n("Edit Speed Dial") + "</span><span class='done'><img>" + i18n("Done") + "</span>";
                    if (this._editing) {
                        li.className = "done_speed_dial";
                    }
                }
                if (list[i].add) {
                    li.className = "add_speed_dial fa fa-plus";
                    /*
                    li.innerHTML = "<img><p class='speeddial_title'></p>";
                    li.querySelector("p").innerText = i18n("Add to Speed Dial");
                    */
                    li.innerHTML = "<img>";
                }
            }
        }
        var selectableLiArray = Array.prototype.slice.call(this.container.querySelectorAll("li") || []);
        if (lastIndex >= 0) {
            KeyboardHandler.setFocus(selectableLiArray[lastIndex]);
        }

    },
    updateSpeedDialThumbnail: function(args) {
        if (this) {
            var index;
            for (index = 0; index < SpeedDial._list.length; index++) {
                if (SpeedDial._list[index].url == args[1]) {
                    if(args[5] == "default"){
                        SpeedDial._list[index].thumbnail = defaultImageSrc;
                        SpeedDial._list[index].title = url;
                    }else{
                        SpeedDial._list[index].thumbnail = args[5];
                        SpeedDial._list[index].title = args[4];
                    }
                    break;
                }
            }
            if(args[5]=="default"){
                this.querySelector("img").src = defaultImageSrc;
            }else{
                this.querySelector("img").src = args[5];
                this.querySelector("p").innerText=args[4];
            }
            this.classList.add("thumbnail-handled");
        }
        Sections.unblockThumbnailMutex();
        if(!document.querySelector(".speeddial").classList.contains("show")){
            console.log("destroyThumbnailWindow");
            NativeApi.destroyThumbnailWindow();
        }else{
            if((!document.querySelector(".speed-dial-entry:not(.thumbnail-handled)"))&&(!document.querySelector(".recommend_item:not(.thumbnail-handled)"))){
                console.log("destroyThumbnailWindow");
                NativeApi.destroyThumbnailWindow();
            }
        }
    },
    show: function() {
        this.element.classList.add("show");
    },
    hide: function() {
        this.element.classList.remove("show");
    },
    enableMoveState: function() {
        this._moving = true;
        //this._movingID = document.querySelector(".speeddials .focus").getAttribute("data-id");
        this._movingID = Popups.prefocused.getAttribute("data-id");
        document.querySelector("#speeddial-caption").innerText = i18n("Move Speed Dial entry");
    },
    disableMoveState: function() {
        this._moving = false;
        document.querySelector("#speeddial-caption").innerText = i18n("Select a Speed Dial entry to edit");
    },
    isInMoveState: function() {
        return this._moving && this._editing;
    },
    isInEditState: function() {
        return this._editing;
    }
}

var Notification = function(text) {
    this.makeNotification();
    this.setData(text);

    if (Notification.stack.length == 0) {
        this.show();
    }
    Notification.stack.push(this);
};
Notification.stack = [];
Notification.prototype = {
    makeNotification: function() {
        this.el = document.querySelector(".notifications").appendChild(document.createElement("div"));
    },
    setData: function(text) {
        this.el.innerHTML = text;
    },
    show: function() {
        NativeApi.gainFocus();
        setTimeout(function() {
            NativeApi.setWindowSize(0, 0, window.innerWidth, 1080,null);
            this.el.classList.add("show");
        }.bind(this), 10);
        setTimeout(function() {
            this.hide();
            /*
            if(KeyboardHandler.getState()==KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION||KeyboardHandler.getState()==KeyboardHandler.states.PAGE_NAVIGATION){
                NativeApi.setWindowSize(0, 0, window.innerWidth, 50,null);
            }
            */
        }.bind(this), SETTINGS.NOTIFICATION_DISPLAY_DURATION);
    },
    hide: function() {
        this.el.classList.remove("show");
        setTimeout(function() {
            NativeApi.blurFocus();
            this.el.parentNode.removeChild(this.el);
            Notification.stack.shift();
            if (Notification.stack.length > 0) {
                Notification.stack[0].show();
            } else {
                NativeApi.blurFocus();
            }
            if(KeyboardHandler.getState()==KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION||KeyboardHandler.getState()==KeyboardHandler.states.PAGE_NAVIGATION){
                NativeApi.setWindowSize(0, 0, window.innerWidth, 50,null);
            }
        }.bind(this), 500);
    }
};

var NativeApi = {
    EventListeners: [],
    CommandCallback: [],
    platformMessageDispatcher: function(event) {
        var o = JSON.parse(event);
        console.log("platformMessageDispatcher = " + o.msg);
        try {
            switch (o.msg) {
                case "getAllBookmarks":
                {
                    var speeddialsString = o.speeddials;
                    var result = [];
                    result[0] = JSON.parse(speeddialsString);
                    result[1] = 0;
                    NativeApi.EventListeners[o.msg](result[0], 0);
                }
                    break;
                case "getSpeedDials":
                    var speeddialsString = o.speeddials;
                    var result = [];
                    result[0] = JSON.parse(speeddialsString);
                    result[1] = 0;
                    NativeApi.EventListeners[o.msg](result[0], 0);
                    break;
                case "getSpeedDialByUrl":
                    var speeddialsString = o.speeddial;
                    var result = JSON.parse(speeddialsString);
                    NativeApi.EventListeners[o.msg](result);
                    break;
                case "getHistory":
                    var historyString = o.historys;
                    if (historyString == "")
                        NativeApi.EventListeners[o.msg](null, 0);
                    else
                        NativeApi.EventListeners[o.msg](JSON.parse(historyString), 0);
                    break;
                case "addSpeedDial":
                case "removeSpeedDial":
                case "moveSpeedDial":
                case "saveBookmark":
                case "deleteBookmark":
                    NativeApi.EventListeners[o.msg](o.url, o.result);
                    break;
                case "modifySpeedDial":
                    NativeApi.EventListeners[o.msg](o.result);
                    break;
                case "createThumbnailForUrl":
                    NativeApi.EventListeners[o.msg](null, o.url, null, null, o.title, o.thumbnail);
                    break;
                case "getCursorPosition":
                    NativeApi.EventListeners[o.msg](o.x, o.y);
                    break;
                case "getCursorAndScroll":
                    NativeApi.EventListeners[o.msg](o.x, o.y,o.scroll_x, o.scroll_y, o.max_scroll_x, o.max_scroll_y);
                    break;
                case "getScroll":
                    if (o.type == "set") {
                        NativeApi.EventListeners[o.msg](o.scroll_x, o.scroll_y, o.max_scroll_x, o.max_scroll_y);
                    }
                    break;
                case "getZoomFactor":
                    NativeApi.EventListeners[o.msg](o.zoomFactor);
                    break;
                case "clearHistory":
                    NativeApi.EventListeners[o.msg](o.clearHistory);
                    break;
                case "windowResizeTo":
                    NativeApi.EventListeners[o.msg](o.result);
                    break;
                case "videoFSChanged":
                    NativeApi.EventListeners[o.msg](o.result);
                    break;
                case "getVirtualKeyBoardStatus":
                    NativeApi.EventListeners[o.msg](o.isShow);
                    break;
                case "virtualKeyboardHide":
                    KeyboardHandler.setNavTimer();
                    break;
                case "loadHomepageURL":
                    console.log("start Borwser by -u ,URL="+o.url);
                    NativeApi.loadUrl(o.url);
                    break;
                case "showNavigation":
                    KeyboardHandler.showNavigation();
                    break;
                 /*case "tabadded":
                    Tabs.addTab(o.id);
                    break;*/
                default:
                    NativeApi.EventListeners[o.msg](o);
                    break;
            }

        } catch (e) {
            //TODO handle the exception
        }
    },
    init: function() {
        try {
            opera_omi.addPlatformEventListener(NativeApi.platformMessageDispatcher);
        } catch (e) {
            //TODO handle the exception
        }
        sendCommand("addEventListener", 'loadingstarted', NativeApi.eventUrlChanged);
        sendCommand("addEventListener", 'loadingfinished', NativeApi.eventLoadingFinished);
        sendCommand("addEventListener", 'loadingfailed', NativeApi.eventLoadingFailed);
        sendCommand("addEventListener", 'historychanged', NativeApi.eventAddHistory);
        sendCommand("addEventListener", 'tabadded', NativeApi.eventAddedTab);
        sendCommand("addEventListener", 'loadingprogress', LoadingBar.progress);
        sendCommand("addEventListener", 'tabactivated', NativeApi.eventTabActivated);
        sendCommand("addEventListener", 'tabremoved', NativeApi.eventTabRemoved);
        sendCommand("addEventListener", 'sslstatechanged', NativeApi.eventSslStateChanged);
        sendCommand("addEventListener", 'dialogshow', NativeApi.eventDialogShow);
        sendCommand("addEventListener", 'topmarginreached', NativeApi.eventTopMarginReached);
        sendCommand("addEventListener", 'titleupdated', NativeApi.eventTitleUpdated);
        sendCommand("addEventListener", 'authenticationdialogshow', NativeApi.eventAuthenticationDialog);
        sendCommand("addEventListener", 'tablimitreached', NativeApi.eventTabLimitReached);
        sendCommand("addEventListener", 'zoomchanged', NativeApi.eventZoomChanged);
        sendCommand("addEventListener", "videoPlayed", NativeApi.videoPlayed);
        sendCommand("addEventListener", "videoFSChanged", NativeApi.videoFSChanged);
        sendCommand("addEventListener", "videoPaused", NativeApi.videoPaused);
        sendCommand("addEventListener", "videoPlaying", NativeApi.videoPlaying);
        sendCommand("addEventListener", "videoWaiting", NativeApi.videoWaiting);
        sendCommand("addEventListener", "videoTimeUpdate", NativeApi.videoTimeUpdate);
        sendCommand("addEventListener", "videoEnded", NativeApi.videoEnded);
    },
    videoPlayed: function(e) {
        //PlayControl._progressbar.style.width = "0%";
        PlayControl._play_pause = false;
        PlayControl._button_array[1].classList.remove("fa-play");
        PlayControl._button_array[1].classList.add("fa-pause");
        PlayControl._playerbarAutoHide();
    },
    videoFSChanged:function(e){
        console.log("videoFSChanged");
        if(e==1){
            console.log("Enter fullScreen");
            if (window.innerHeight != 1080) {
                NativeApi.setWindowSize(0, 0, window.innerWidth,1080,null);
            }
            document.querySelector("#status-bar").classList.add("hide");
            document.querySelector(".header").classList.add("anihide");
            NativeApi.show();
            NativeApi.gainFocus();
            PlayControl._show();
            PlayControl._button_array[1].classList.add("fa-pause");
            PlayControl._time_infos[0].innerText= "00:00:00";
            PlayControl._time_infos[1].innerText= "00:00:00";
            KeyboardHandler.setState(KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY);
        }else{
            PlayControl._button_array[3].click();
        }
    },
    videoPaused:function(){
        PlayControl._play_pause = true;
        PlayControl._button_array[1].classList.remove("fa-pause");
        PlayControl._button_array[1].classList.add("fa-play");
        if (PlayControl._play_pause && PlayControl._hideTimer != null) {
            window.clearTimeout(PlayControl._hideTimer);
        }
    },
    videoPlaying:function(){
        PlayControl._browserLoading.classList.remove("browser-player-show");
        PlayControl._browserLoading.classList.add("browser-player-hide");
    },
    videoWaiting:function(){
        PlayControl._browserLoading.classList.remove("browser-player-hide");
        PlayControl._browserLoading.classList.add("browser-player-show");
    },
    videoEnded:function(){
        PlayControl._button_array[3].click();
    },
    videoTimeUpdate:function(e){
        PlayControl._progressbar.style.width = 70 * e.progress + "%";
        if(e.progress<0){
            PlayControl._time_infos[0].innerText= "00:00:00";
        }else{
            PlayControl._time_infos[0].innerText= e.curTime;
        }
        PlayControl._time_infos[1].innerText= e.totalTime;
    },

    eventAddHistory: function(e) {
        LOG("eventHistoryChanged", e);
        if (e.url && !isCustomErrorPages(e.url)) {
            Application.onNewEntry(e.url, e.history_index, e.history_size, e.tab_id);
            //Tabs.updateTabUrl(e.tab_id, e.url);
        }
    },
    eventUrlChanged: function(e) {
        SpeedDial.render(); 
        //KeyboardHandler.setFocus(speeddialNewfocus, 0);
        LOG("eventLoadingStarted", e);
        if (e.url) {
            if(e.url == "about:blank"){
                document.getElementById("reloadStop").classList.add("disabled");
                //document.querySelector("#status-bar").classList.remove("hide");
                document.querySelector(".header").classList.remove("anihide");
                Application.zoom=1;
                NativeApi.setZoom(1 * 1.5);
                document.querySelector("#zoom").classList.remove("fa-cc-discover");
                document.querySelector("#zoom").classList.remove("fa-cc-mastercard");
                document.querySelector("#zoom").classList.add("fa-cc-visa");
                document.getElementById("zoom").classList.add("disabled");

                PlayControl._playControlDisplayNone();
            } else {
                Popups.close();
                document.getElementById("back").classList.remove("disabled");
                document.getElementById("reloadStop").classList.remove("disabled");
                document.getElementById("zoom").classList.remove("disabled");
                document.querySelector(".header").style.cssText += "-webkit-transition: all 1s;";
            }
            LoadingBar.show();
            currentUrl = e.url;
            if (currentUrl == HISENSE_DEBUG_URL)
                currentUrl = "hisense://debug";
            Application.updateUrl(currentUrl, e.tab_id);
            Tabs.updateTabUrl(e.tab_id, currentUrl);
            Tabs.updateTabTitle(e.tab_id, currentUrl);
            var multitabs = document.getElementById("multi-tabs").getElementsByTagName("button");
        	multitabs[e.tab_id].setAttribute("loadstate", 1);
        	if(Tabs.getActiveTabId() == e.tab_id){
        		document.getElementById("loading_status").classList.remove("hide");
        	}
        }
    },
    eventLoadingFinished: function(e) {
        LOG("eventLoadingFinished", e);
        NativeApi.getZoom(e.tab_id, function(res) {
            console.log("Setting Page Zoom Level to: " + res[0]);
            Tabs.getTab(e.tab_id).setZoom(res[0]);
        });
        var multitabs = document.getElementById("multi-tabs").getElementsByTagName("button");
        multitabs[e.tab_id].setAttribute("loadstate", 0);
        /*
        NativeApi.getTabTitle(function(i) {
            Tabs.updateTabTitle(+e.tab_id, i[0].title);
        }, +e.tab_id);
        */
        if (SETTINGS.SHOW_THUMBNAILS_IN_TABS) {
            NativeApi.createThumbnail(e.tab_id, SETTINGS.THUMBNAIL_WIDTH, SETTINGS.THUMBNAIL_HEIGHT, Tabs.updateTabThumbnail);
        }
        if(Tabs.getActiveTabId() == e.tab_id){
        	document.getElementById("loading_status").classList.add("hide");
        }
    },
    eventLoadingFailed:function(e){
        LOG("eventLoadingFailed", e);
        Tabs.updateTabTitle(0, "unknown");
    },
    eventAddedTab: function(e) {
        LOG("eventAddedTab", e);
        Tabs.addTab(e.tab_id,e.url);
    },
    eventTabActivated: function(e) {
        LOG("eventTabActivated", e);
        LoadingBar.hide();
        Application.updateUrl(Tabs.getTab(e.id).getUrl(), e.id);
        /* */
        /*var _active_tab_id = Tabs.getActiveTabId();
        NativeApi.removeTab(_active_tab_id);*/
        Tabs.setActiveTab(e.id);
        KeyboardHandler.setFocus(Sections.tabsContainer.querySelector(".tab[data-id=\"" + Tabs.getActiveTabId() + "\"]"), Sections.tabsContainer.querySelector(".focus"));
    },
    eventTabRemoved: function(e) {
        LOG("eventTabRemoved", e);
        if (e.id == (document.getElementById("multi-tabs").getElementsByTagName("button").length - 1)) 
        {
        	NativeApi.activateTab(e.id-1);
        	Tabs.removeTab(e.id);
        }else{
        	Tabs.removeTab(e.id);
        	NativeApi.activateTab(e.id);
        }
    },
    eventSslStateChanged: function(e) {
        LOG("eventSslStateChanged", e);
        Application.updateSecurity(+e.state, +e.is_ev_ssl, +e.tab_id);
    },
    eventDialogShow: function(e) {
        switch (e.type) {
            case 0:
                Popups.showAlertPopup(e.message, e.tab_id);
                break;
            case 1:
                Popups.showConfirmPopup(e.message, e.tab_id);
                break;
            case 2:
                Popups.showPromptPopup(e.message, e.default_prompt, e.tab_id);
                break;
        }
    },

    eventTopMarginReached: function(e) {
        if(document.querySelector(".speeddial").classList.contains("show")){
            return;
        }
        if (!KeyboardHandler.isUserInteractingWithKeyboard() && KeyboardHandler.getState() !== KeyboardHandler.states.POPUP_NAVIGATION && KeyboardHandler.getState()!=KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY) {
            var item = document.elementFromPoint(e.x, 50);
            if (item != null) {
                if (item.nodeName == "BUTTON" || item.nodeName == "INPUT") {
                    KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, item);
                } else {
                    KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, document.querySelector("#url"));
                }
            } else {
                KeyboardHandler.setState(KeyboardHandler.states.MENU_NAVIGATION, document.querySelector("#url"));
            }
        }
    },

    eventTitleUpdated: function(e) {
        Tabs.updateTabTitle(+e.tab_id, e.title);
    },
    eventAuthenticationDialog: function(e) {
        Popups.showAuthenticationDialog(e.tab_id);
    },
    eventTabLimitReached: function(e) {
        new Notification(i18n("You cannot open additional tabs"));
    },
    authenticationReceived: function(tab_id, okPressed, login, password) {
        sendCommand("authenticationReceived", +tab_id, okPressed, login, password);
    },
    eventZoomChanged: function(e) {
        LOG("Zoom changed: ", e);
        new Notification(i18n("100% Zoom").replace(i18n.convertNumber(100), i18n.convertNumber(Math.round(e.factor * 100))));
        Tabs.getTab(e.tab_id).setZoom(e.factor);
    },
    addTab: function(url) {
        sendCommand("addTab", url);
    },
    removeTab: function(id) {
        sendCommand("removeTab", +id);
    },
    activateTab: function(id) {
        sendCommand("activateTab", +id);
    },
    loadUrl: function(url) {
        if (url) {
            var trueURL = url;
            if (url == "hisense://debug")
                trueURL = HISENSE_DEBUG_URL;
            sendCommand("loadUrl", trueURL);
        }
    },
    scroll: function(x, y) {
        sendCommand("scroll", x, y);
    },
    scrollUp:function(){
        sendCommand("scrollUp");
    },
    scrollDown:function(){
        sendCommand("scrollDown");
    },
    mouseClick: function(x, y, button) {
        sendCommand("mouseClick", x, y, button);
    },
    mouseMove: function(x, y) {
        sendCommand("mouseMove", x, y);
    },
    moveCursor: function(x, y) {
        var viewSize = Application.getWindowSize();
        sendCommand("moveCursor", Math.min(Math.max(0, x), viewSize.width), Math.min(Math.max(0, y), viewSize.height));
    },
    getScroll: function(callback) {
        sendCommand("getScroll", makeCallback(callback));
    },
    showMouseCursor: function() {
        sendCommand("showMouseCursor");
    },
    hideMouseCursor: function() {
        sendCommand("hideMouseCursor");
    },
    setZoom: function(zoomlevel) {
        sendCommand("zoom", zoomlevel);
    },
    getZoom: function(tab_id, callback) {
        sendCommand("getZoomFactor", tab_id, makeCallback(callback));
    },
    historyMove: function(direction) {
        sendCommand("historyMove", direction);
    },
    reload: function() {
        sendCommand("reload");
    },
    stopLoad: function() {
        sendCommand("stopLoad");
    },
    gainFocus: function() {
        //TODO:UIFocused???
        //if (!this.UIFocused) {
            sendCommand("focus");
        //    this.UIFocused = true;
        //}
    },
    blurFocus: function() {
        // Blur only for PAGE_NAVIGATION, GRID_NAVIGATION or VIRTUAL_MOUSE_NAVIGATION
        if ([KeyboardHandler.states.PAGE_NAVIGATION,
            KeyboardHandler.states.GRID_NAVIGATION,
            KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION
        ].indexOf(KeyboardHandler.getState()) > -1 && Notification.stack.length == 0 /*&& !LoadingBar.isShown()*/) {
            this.UIVisible = false;
           // this.UIFocused = false;
            sendCommand("blur");
        }
    },
    exitFullScreen:function(){
        sendCommand("exitFullScreen");
    },
    show: function() {
        if (!this.UIVisible) {
            sendCommand("show");
            this.UIVisible = true;
        }
    },
    getWindowSize: function(callback) {
        sendCommand("getWindowSize", makeCallback(callback));
    },
    getCursorPosition: function(callback) {
        sendCommand("getCursorPosition", makeCallback(callback));
    },
    getCursorAndScroll:function(callback){
        sendCommand("getCursorAndScroll", makeCallback(callback));
    },
    getTabTitle: function(callback, id) {
        sendCommand("getTabTitle", makeCallback(callback), id);
    },
    getCertificateInfo: function(callback, id) {
        sendCommand("getCertificateInfo", makeCallback(callback), id);
    },
    isVirtualKeyboardVisible: function(callback) {
        sendCommand("isVirtualKeyboardVisible", makeCallback(callback));
    },
    createThumbnail: function(id, width, height, callback) {
//      sendCommand("createThumbnail", id, width, height, makeCallback(callback));
    },
    createThumbnailForUrl: function(url, width, height, callback) {
        sendCommand("createThumbnailForUrl", url, width, height, makeCallback(callback));
    },
    destroyThumbnailWindow:function(){
        sendCommand("destroyThumbnailWindow");
    },
    saveBookmark: function(url, title, callback) {
        sendCommand("saveBookmark", url, title, makeCallback(callback));
    },
    moveBookmark: function(url, position, callback) {
        sendCommand("moveBookmark", url, position, makeCallback(callback));
    },
    deleteBookmark: function(url, title, callback) {
        sendCommand("deleteBookmark", url, title, makeCallback(callback));
    },
    deleteHistory:function(url, time, callback){
        sendCommand("deleteHistory", url, time, makeCallback(callback));
    },
    clearCookies:function(){
        sendCommand("clearCookies");
    },
    getAllBookmarks: function(callback) {
        sendCommand("getAllBookmarks", makeCallback(callback));
    },
    getHistory: function(callback) {
        sendCommand("getHistory", makeCallback(callback),arguments[1]);
    },
    clearHistory: function(callback) {
        sendCommand("clearHistory", makeCallback(callback));
    },
    getSpeedDials: function(callback) {
        sendCommand("getSpeedDials", makeCallback(callback));
    },
    getSpeedDialByUrl:function(url,callback){
        sendCommand("getSpeedDialByUrl",url,makeCallback(callback));
    },
    addSpeedDial: function(url, title, thumbnail,callback) {
        sendCommand("addSpeedDial", url, title, thumbnail,makeCallback(callback));
    },
    moveSpeedDial: function(url, position, callback) {
        sendCommand("moveSpeedDial", url, position, makeCallback(callback));
    },
    removeSpeedDial: function(url, callback) {
        sendCommand("removeSpeedDial", url, makeCallback(callback));
    },
    modifySpeedDial:function(url,newUrl,newTitle,callback){
        sendCommand("modifySpeedDial",url,newUrl,newTitle,makeCallback(callback));
    },
    dialogClosed: function(id, success, user_input) {
        sendCommand("dialogclosed", id, success || false, user_input || "");
    },
    getSettings: function(callback) {
        sendCommand("getSettings", makeCallback(callback));
    },
    getHighlightedUrl: function(callback) {
        sendCommand("getHighlightedUrl", makeCallback(callback));
    },
    changePreferences: function(enable_cookies, speeddial_width, speeddial_height) {
        sendCommand("changePreferences", enable_cookies, speeddial_width, speeddial_height);
    },
    isFeatureEnabled: function(name, callback) {
        sendCommand("isFeatureEnabled", name, makeCallback(callback));
    },
    setWindowSize: function(x, y, width, height,callback) {
        sendCommand("windowResizeTo", x, y, width, height,makeCallback(callback));
    },
    focusMainWindow: function() {
        sendCommand("focusMainWindow");
    },
    getVirtualKeyBoardStatus: function(callback){
        sendCommand("getVirtualKeyBoardStatus",makeCallback(callback));
    }
};

function changeUrl(url) {
    NativeApi.loadUrl(url);
}

function LOG(message, e) {
    console.log(message + ": " + JSON.stringify(e));
}

function findParentContainingClass(whatClass, whatElement) {
    try{
        while (whatElement && whatElement.classList && whatElement != document.body && !whatElement.classList.contains(whatClass)) {
            if(whatElement.parentNode){
                whatElement = whatElement.parentNode;
            }
        }
        return (whatElement.classList && whatElement.classList.contains(whatClass)) ? whatElement : null;
    }catch(e){
        console.log(e);
        return null;
    }


}

function isCustomErrorPages(url) {
    var n = url.indexOf('?');
    var current_url = url.substring(0, n == -1 ? url.length : n).toLowerCase();
    if (!(SETTINGS.CUSTOM_ERROR_PAGES && SETTINGS.CUSTOM_ERROR_PAGES.length)) {
        return false;
    }
    for (var i = 0; i < SETTINGS.CUSTOM_ERROR_PAGES.length; i++) {
        if (current_url == SETTINGS.CUSTOM_ERROR_PAGES[i].toLowerCase())
            return true;
    }
    return false;
}


MouseHandler = {
    init: function() {
        Application.getWindowSize(function(viewSize) {
            NativeApi.moveCursor(viewSize.width / 2 | 0, viewSize.height / 2 | 0);
            NativeApi.hideMouseCursor();
        });

        window.addEventListener("click", function(e) {
            var clickedSpeeddial = findParentContainingClass("speeddial", e.target);
            var state = KeyboardHandler.getState();
            if (e.target == document.body || clickedSpeeddial) {
                if (!clickedSpeeddial && state == KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION) {
                    NativeApi.mouseClick(e.x, e.y, 0);
                    e.preventDefault();
                }
                if (state == KeyboardHandler.states.MENU_NAVIGATION) {
                    if (clickedSpeeddial) {
                        KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
                    } else {
                        KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
                    }
                }
            } else {
                e.target.classList.add("clicked");
                setTimeout(function() {
                    e.target.classList.remove("clicked");
                }, SETTINGS.PRESSED_STATE_DURATION);
            }
            if (e.target.parentNode.id == "history_date") {
                document.querySelector("#history_date .focused").classList.remove("focused");
                e.target.classList.add("focused");
            }
            if (e.target.nodeName == "A") {
                e.preventDefault();
                Popups.closeAll();
                NativeApi.loadUrl(e.target.getAttribute("href"));
            }
        if(e.target.id == "url"
            || e.target.id == "add-to-speed-dial-title"
            || e.target.id == "add-to-speed-dial-address") {
                e.target.blur();
                e.target.focus();
            }
        }, false);

        window.addEventListener("mouseover", function(e) {
            if (KeyboardHandler.isUserInteractingWithKeyboard()) {
                return;
            }
            var element = e.target;
            if (!element.hasAttribute("tabindex") && element.parentNode) {
                element = element.parentNode;
            }
            if (element.hasAttribute("tabindex")) {
                var oldFocus = document.querySelector(".active .focus");
                var rootNode = findParentContainingClass("line", element);
                if (rootNode && rootNode.classList && !rootNode.classList.contains("active")) {
                    var active = document.querySelectorAll(".active");
                    if (active.length>0) {
                        var index=0;
                        while(index<active.length){
                            active[index].classList.remove("active");
                            index++;
                        }
                    }
                    if (oldFocus) {
                        oldFocus.classList.remove("focus");
                    }
                    rootNode.classList.add("active");
                    //oldFocus = document.querySelector(".active .focus");
                }
                //TODO?
                /*
                var focusGroup = element.getAttribute("data-focus-group");
                if (focusGroup) {
                    oldFocus = document.querySelector("." + focusGroup + " .focus");
                }
                */
                if(element!=oldFocus){
                    KeyboardHandler.setFocus(element, oldFocus);
                }
                MouseHandler.switchState(element, oldFocus);
                if(Popups._openedPopup&&Popups.prefocused&&!Popups.prefocused.classList.contains("focus")){
                    Popups.prefocused.classList.add("focus");
                }
            }
            if(findParentContainingClass("speeddials", element)){
                document.querySelector(".speeddial").classList.add("active");
            }
            if(!document.querySelector(".speeddial").classList.contains("show")&&KeyboardHandler.getState()==KeyboardHandler.states.MENU_NAVIGATION){
                KeyboardHandler.setNavTimer();
            }
        }, false);
        document.querySelector("#history_detail").addEventListener("wheel", function(e) {
            if (document.querySelectorAll("#history_detail li.hide").length > 0) {
                return;
            }

            NativeApi.showMouseCursor();
            if (e.wheelDeltaY < 0) {
                console.log("Wheel Down");
                if (Sections.getHistoryIndex() + 9 < Sections.getHistoryLength()) {
                    Sections.renderHistory(Sections.getHistoryIndex() + 1);
                }
            } else {
                console.log("Wheel Up");
                if (Sections.getHistoryIndex() > 0) {
                    Sections.renderHistory(Sections.getHistoryIndex() - 1);
                }
            }
        }, false);
        document.querySelector(".speeddials-anim-wrapper").addEventListener("wheel", function(e) {
            if (document.querySelectorAll(".speeddials ul").length == 1) {
                return;
            }

            NativeApi.showMouseCursor();
            var ULArray = document.querySelectorAll(".speeddials ul");
            var currentLI = document.querySelector(".speeddials li.focus");
            var currentUL = currentLI.parentNode;
            var currentLIArray = Array.apply(null, currentUL.querySelectorAll("li"));
            var currentLIIndex = currentLIArray.indexOf(currentLI);
            var currentULIndex = parseInt(currentUL.getAttribute("data-start-index")) / 12;
            if (e.wheelDeltaY < 0) {
                console.log("Wheel Down");
                if (currentULIndex + 1 <= ULArray.length - 1) {
                    var nextLIArray = Array.apply(null, ULArray[currentULIndex + 1].querySelectorAll("li"));
                    var newFocusLI;
                    if (nextLIArray[currentLIIndex]) {
                        newFocusLI = nextLIArray[currentLIIndex];
                    } else {
                        newFocusLI = nextLIArray[nextLIArray.length - 1];
                    }
                    KeyboardHandler.setFocus(newFocusLI, currentLI);
                    KeyboardHandler.updateFocusInSpeedDialScrollableArea(newFocusLI);
                    ScrollBarManager.speedDialScrollBar.moveScrollBar(currentULIndex + 1);
                }

            } else {
                console.log("Wheel Up");
                if (currentULIndex - 1 >= 0) {
                    var priLIArray = Array.apply(null, ULArray[currentULIndex - 1].querySelectorAll("li"));
                    KeyboardHandler.setFocus(priLIArray[currentLIIndex], currentLI);
                    KeyboardHandler.updateFocusInSpeedDialScrollableArea(priLIArray[currentLIIndex]);
                    ScrollBarManager.speedDialScrollBar.moveScrollBar(currentULIndex - 1);
                }
            }
        }, false);
    },

    switchState: function(newFocus, oldFocus) {
        var newstate;
        newstate = KeyboardHandler.states[newFocus.dataset.state];
        if (typeof (newstate) == "undefined") {
            newstate = KeyboardHandler.states.POPUP_NAVIGATION;
        }
        KeyboardHandler.state = newstate;
    }
}

var MousePosition = {
    x: 0,
    y: 0,
    set: function(x, y) {
        this.x = x;
        this.y = y;
    },
    get: function() {
        return {
            x: this.x,
            y: this.y
        };
    }
};

var CommandType = {
    CMD_ID_INVALID: 0,
    CMD_ID_AUTHENTICATIONRECEIVED: 1,
    CMD_ID_ADDTAB: 2,
    CMD_ID_REMOVETAB: 3,
    CMD_ID_ACTIVATETAB: 4,
    CMD_ID_LOADURL: 5,
    CMD_ID_SCROLL: 6,
    CMD_ID_MOUSECLICK: 7,
    CMD_ID_MOUSEMOVE: 8,
    CMD_ID_MOVECURSOR: 9,
    CMD_ID_GETSCROLL: 10,
    CMD_ID_SHOWMOUSECURSOR: 11,
    CMD_ID_HIDEMOUSECURSOR: 12,
    CMD_ID_ZOOM: 13,
    CMD_ID_GETZOOMFACTOR: 14,
    CMD_ID_HISTORYMOVE: 15,
    CMD_ID_RELOAD: 16,
    CMD_ID_STOPLOAD: 17,
    CMD_ID_FOCUS: 18,
    CMD_ID_BLUR: 19,
    CMD_ID_SHOW: 20,
    CMD_ID_GETWINDOWSIZE: 21,
    CMD_ID_GETCURSORPOSITION: 22,
    CMD_ID_GETTABTITLE: 23,
    CMD_ID_GETCERTIFICATEINFO: 24,
    CMD_ID_ISVIRUTALKEYBOARDVISIBLE: 25,
    CMD_ID_CREATETHUMBNAIL: 26,
    CMD_ID_CREATETHUMBNAILFORURL: 27,
    CMD_ID_SAVEBOOKMARK: 28,
    CMD_ID_MOVEBOOKMARK: 29,
    CMD_ID_DELETEBOOKMARK: 30,
    CMD_ID_GETALLBOOKMARKS: 31,
    CMD_ID_GETHISTORY: 32,
    CMD_ID_CLEARHISTORY: 33,
    CMD_ID_GETSPEEDDIALS: 34,
    CMD_ID_ADDSPEEDDIAL: 35,
    CMD_ID_MOVESPEEDDIAL: 36,
    CMD_ID_REMOVESPEEDDIAL: 37,
    CMD_ID_DIALOGCLOSED: 38,
    CMD_ID_GETSETTINGS: 39,
    CMD_ID_GETHIGHLIGHTEDURL: 40,
    CMD_ID_CHANGEPREFERENCES: 41,
    CMD_ID_ISFEATUREENABLED: 42,
    CMD_ID_WINDOWRESIZETO: 45,
    CMD_ID_DELETEHISTORY: 47,
    CMD_ID_CLEARCOOKIES: 48,
    CMD_ID_FOCUS_MAIN_WINDOW: 50,
    CMD_ID_GETCURSORANDSCROLL: 51,
    CMD_ID_MODIFYSPEEDDIAL: 54,
    CMD_ID_VIDEOBACKWARD: 55,
    CMD_ID_VIDEOPLAYPAUSE: 56,
    CMD_ID_VIDEOSTOP: 57,
    CMD_ID_VIDEOFORWARD: 58,
    CMD_ID_VIDEO_SET_PERCENTAGE: 64,
    CMD_ID_GETSPEEDDIALBYURL:65,
    CMD_ID_GETVIRTUALKEYBOARDSTATUS:66,
    CMD_ID_DESTROYTHUMBNAILWINDOW:67,
    CMD_ID_EXITFULLSCREEN:68
};

function sendCommand(cmd, args) {
    var cmdInfo = {};
    switch (arguments[0]) {
        case "addEventListener":
            NativeApi.EventListeners[arguments[1]] = arguments[2];
            break;
        case "authenticationReceived": //("authenticationReceived", +tab_id, okPressed, login, password);
            cmdInfo.type = CommandType.CMD_ID_AUTHENTICATIONRECEIVED;
            cmdInfo.tab_id = arguments[1];
            cmdInfo.okPressed = arguments[2];
            cmdInfo.password = arguments[3];
            break;
        case "addTab": //("addTab", url);
            cmdInfo.type = CommandType.CMD_ID_ADDTAB;
            cmdInfo.url = arguments[1];
            break;
        case "removeTab": //("removeTab", +id);
            cmdInfo.type = CommandType.CMD_ID_REMOVETAB;
            cmdInfo.id = arguments[1];
            break;
        case "activateTab": //("activat\neTab", +id);
            cmdInfo.type = CommandType.CMD_ID_ACTIVATETAB;
            cmdInfo.id = arguments[1];
            break;
        case "loadUrl": //("loadUrl", url);
            cmdInfo.type = CommandType.CMD_ID_LOADURL;
            cmdInfo.url = arguments[1];
            break;
        case "scroll": //("scroll", x, y);
            cmdInfo.type = CommandType.CMD_ID_SCROLL;
            cmdInfo.x = arguments[1];
            cmdInfo.y = arguments[2];
            break;
        case "scrollUp":
            cmdInfo.type = CommandType.CMD_ID_SCROLL_UP;
            break;
        case "scrollDown":
            cmdInfo.type = CommandType.CMD_ID_SCROLL_DOWN;
            break;
        case "mouseClick": //("mouseClick", x, y, button);
            cmdInfo.type = CommandType.CMD_ID_MOUSECLICK;
            cmdInfo.x = arguments[1];
            cmdInfo.y = arguments[2];
            cmdInfo.button = arguments[3];
            break;
        case "mouseMove": //("mouseMove", x, y);
            cmdInfo.type = CommandType.CMD_ID_MOUSEMOVE;
            cmdInfo.x = arguments[1]
            cmdInfo.y = arguments[2]
            break;
        case "moveCursor": //("moveCursor", Math.min(Math.max(0, x), viewSize.width), Math.min(Math.max(0, y), viewSize.height));
            cmdInfo.type = CommandType.CMD_ID_MOVECURSOR;
            cmdInfo.x = arguments[1]
            cmdInfo.y = arguments[2]
            break;
        case "getScroll": //("getScroll", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
//          NativeApi.EventListeners[arguments[0]](0, 0, 1920, 1080);
            cmdInfo.type = CommandType.CMD_ID_GETSCROLL;
            break;
        case "showMouseCursor":
            cmdInfo.type = CommandType.CMD_ID_SHOWMOUSECURSOR;
            break;
        case "hideMouseCursor":
            cmdInfo.type = CommandType.CMD_ID_HIDEMOUSECURSOR;
            break;
        case "zoom": //("zoom", zoomlevel);
            cmdInfo.type = CommandType.CMD_ID_ZOOM;
            cmdInfo.zoomlevel = arguments[1]
            break;
        case "getZoomFactor": //("getZoomFactor", tab_id, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[2]];
            cmdInfo.type = CommandType.CMD_ID_GETZOOMFACTOR;
            cmdInfo.tab_id = arguments[1];
            break;
        case "historyMove": //("historyMove", direction);
            cmdInfo.type = CommandType.CMD_ID_HISTORYMOVE;
            cmdInfo.direction = arguments[1];
            break;
        case "reload":
            cmdInfo.type = CommandType.CMD_ID_RELOAD;
            break;
        case "stopLoad":
            cmdInfo.type = CommandType.CMD_ID_STOPLOAD;
            break;
        case "focus":
            cmdInfo.type = CommandType.CMD_ID_FOCUS;
            break;
        case "blur":
            cmdInfo.type = CommandType.CMD_ID_BLUR;
            break;
        case "show":
            cmdInfo.type = CommandType.CMD_ID_SHOW;
            break;
        case "getWindowSize": //("getWindowSize", makeCallback(callback));
            this[arguments[1]](document.body.offsetWidth, document.body.offsetHeight);
//          cmdInfo.type = CommandType.CMD_ID_GETWINDOWSIZE;
            break;
        case "getCursorPosition": //("getCursorPosition", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_GETCURSORPOSITION;
            break;
        case "getCursorAndScroll":
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_GETCURSORANDSCROLL;
            break;
        case "getTabTitle": //("getTabTitle", makeCallback(callback), id);
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
//          NativeApi.EventListeners[arguments[0]]("", "")
            cmdInfo.type = CommandType.CMD_ID_GETTABTITLE;
            cmdInfo.id = arguments[2];
            break;
//      case "getCertificateInfo": //("getCertificateInfo", makeCallback(callback), id);
//          NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
//          cmdInfo.type = CommandType.CMD_ID_GETCERTIFICATEINFO;
//          cmdInfo.id = arguments[2];
//          break;
        case "isVirtualKeyboardVisible": //("isVirtualKeyboardVisible", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            NativeApi.EventListeners[arguments[0]](false);
            cmdInfo.type = CommandType.CMD_ID_ISVIRUTALKEYBOARDVISIBLE;
            break;
        case "createThumbnail": //("createThumbnail", id, width, height, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_CREATETHUMBNAIL;
            cmdInfo.id = arguments[1];
            cmdInfo.width = arguments[2];
            cmdInfo.height = arguments[3];
            break;
        case "createThumbnailForUrl": //("createThumbnailForUrl", url, width, height, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[4]];
            cmdInfo.type = CommandType.CMD_ID_CREATETHUMBNAILFORURL;
            cmdInfo.url = arguments[1];
            cmdInfo.width = arguments[2];
            cmdInfo.height = arguments[3];
            break;
        case "destroyThumbnailWindow":
            cmdInfo.type=CommandType.CMD_ID_DESTROYTHUMBNAILWINDOW;
            break;
        case "saveBookmark": //("saveBookmark", url, title, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[3]];
            cmdInfo.type = CommandType.CMD_ID_SAVEBOOKMARK;
            cmdInfo.url = arguments[1];
            cmdInfo.title = arguments[2];
            break;
        case "moveBookmark": //("moveBookmark", url, position, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[3]];
            cmdInfo.type = CommandType.CMD_ID_MOVEBOOKMARK;
            cmdInfo.url = arguments[1];
            cmdInfo.position = arguments[2];
            break;
        case "deleteBookmark": //("deleteBookmark", url, title, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[3]];
            cmdInfo.type = CommandType.CMD_ID_DELETEBOOKMARK;
            cmdInfo.url = arguments[1];
            cmdInfo.title = arguments[2];
            break;
        case "deleteHistory": //("deleteHistory", url, time, makeCallback(back));
            NativeApi.EventListeners[arguments[0]] = this[arguments[3]];
            cmdInfo.type = CommandType.CMD_ID_DELETEHISTORY;
            cmdInfo.url = arguments[1];
            cmdInfo.time = arguments[2];
            break;
        case "clearCookies":
            cmdInfo.type = CommandType.CMD_ID_CLEARCOOKIES;
            break;
        case "getAllBookmarks": //("getAllBookmarks", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_GETALLBOOKMARKS;
            break;
        case "getHistory": //("getHistory", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_GETHISTORY;
            cmdInfo.top=arguments[2];
            break;
        case "clearHistory": //("clearHistory", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_CLEARHISTORY;
            break;
        case "getSpeedDials": //("getSpeedDials", makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
            cmdInfo.type = CommandType.CMD_ID_GETSPEEDDIALS;
            break;
        case "getSpeedDialByUrl":
            NativeApi.EventListeners[arguments[0]]=this[arguments[2]];
            cmdInfo.type=CommandType.CMD_ID_GETSPEEDDIALBYURL;
            cmdInfo.url=arguments[1];
            break;
        case "addSpeedDial": //("addSpeedDial", url, title, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[4]];
            cmdInfo.type = CommandType.CMD_ID_ADDSPEEDDIAL;
            cmdInfo.url = arguments[1];
            cmdInfo.title = arguments[2];
            cmdInfo.thumbNail=arguments[3];
            break;
        case "moveSpeedDial": //("moveSpeedDial", url, position, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[3]];
            cmdInfo.type = CommandType.CMD_ID_MOVESPEEDDIAL;
            cmdInfo.url = arguments[1]
            cmdInfo.position = arguments[2]
            break;
        case "removeSpeedDial": //("removeSpeedDial", url, makeCallback(callback));
            NativeApi.EventListeners[arguments[0]] = this[arguments[2]];
            cmdInfo.type = CommandType.CMD_ID_REMOVESPEEDDIAL;
            cmdInfo.url = arguments[1]
            break;
        case "modifySpeedDial":
            NativeApi.EventListeners[arguments[0]]=this[arguments[4]];
            cmdInfo.type=CommandType.CMD_ID_MODIFYSPEEDDIAL;
            cmdInfo.url=arguments[1];
            cmdInfo.newUrl=arguments[2];
            cmdInfo.newTitle=arguments[3];
            break;
        case "dialogclosed": //("dialogclosed", id, success || false, user_input || "");
            cmdInfo.type = CommandType.CMD_ID_DIALOGCLOSED;
            cmdInfo.id = arguments[1]
            cmdInfo.success = arguments[2]
            cmdInfo.user_input = arguments[3]
            break;
//      case "getSettings": //("getSettings", makeCallback(callback));
//          NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
//          cmdInfo.type = CommandType.CMD_ID_GETSETTINGS;
//          break;
//      case "getHighlightedUrl": //("getHighlightedUrl", makeCallback(callback));
//          NativeApi.EventListeners[arguments[0]] = this[arguments[1]];
//          NativeApi.EventListeners[arguments[0]]("");
//          cmdInfo.type = CommandType.CMD_ID_GETHIGHLIGHTEDURL;
//          break;
        case "windowResizeTo":
            NativeApi.EventListeners[arguments[0]] = this[arguments[5]];
            cmdInfo.type = CommandType.CMD_ID_WINDOWRESIZETO;
            cmdInfo.x = arguments[1];
            cmdInfo.y = arguments[2];
            cmdInfo.width = arguments[3];
            cmdInfo.height = arguments[4];
            break;
        case "focusMainWindow":
            cmdInfo.type = CommandType.CMD_ID_FOCUS_MAIN_WINDOW;
            break;
        case "video_backward":
            cmdInfo.type = CommandType.CMD_ID_VIDEOBACKWARD;
            break;
        case "video_play_pause":
            cmdInfo.type = CommandType.CMD_ID_VIDEOPLAYPAUSE;
            break;
        case "video_forward":
            cmdInfo.type = CommandType.CMD_ID_VIDEOFORWARD;
            break;
        case "video_stop":
            cmdInfo.type = CommandType.CMD_ID_VIDEOSTOP;
            break;
        case "exit_browser":
            cmdInfo.BrowserControl = 1;
            break;
        case "setVideoPercentage":
            cmdInfo.type = CommandType.CMD_ID_VIDEO_SET_PERCENTAGE;
            cmdInfo.percentage = arguments[1];
            break;
        case "getVirtualKeyBoardStatus":
            NativeApi.EventListeners[arguments[0]]=this[arguments[1]];
            cmdInfo.type=CommandType.CMD_ID_GETVIRTUALKEYBOARDSTATUS;
            break;
        case "exitFullScreen":
            cmdInfo.type=CommandType.CMD_ID_EXITFULLSCREEN;
            break;
        default:
            break;
    }
    try {
        if (cmdInfo.type||cmdInfo.BrowserControl) {
            opera_omi.sendPlatformMessage(JSON.stringify(cmdInfo));
        }
    } catch (e) {
        //TODO handle the exception
    }
}

function makeCallback(callback) {
    var funcName = "func" + Date.now() + ((Math.random() * 100) | 0);

    window[funcName] = function() {
        callback(arguments);
        setTimeout(function() {
            window[funcName] = null;
            delete window[funcName];
        }, 1);
    };
    return funcName;
}

function loadResource(a) {
    console.log(a)
    try {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", a, false);
        xhr.send();
    } catch (e) {}
    return xhr;
}

window.location.__GET__ = null;
window.location.__defineGetter__("GET", function() {
    if (!this.__GET__) {
        var GET = {};

        if (this.search) {
            var search = this.search.substr(1); // get rid of "?"
            search.split("&").forEach(function(vk) {
                vk = vk.split("=");
                GET[vk[0]] = vk[1];
            });
        }

        this.__GET__ = GET;
    }
    return this.__GET__;
});

var HomePageManager = {
    state: null,
    states: {
        BOOKMARK_PAGE: 1,
        HISTORY_PAGE: 2
    },
    init: function() {
        this.bookmarkPage = document.getElementById("bookmark_page");
        this.historyPage = document.getElementById("history_page");
        this._historyHide();
        this._bookmarkShow();
        this.state = this.states.BOOKMARK_PAGE;
    },
    _bookmarkHide: function() {
        document.querySelector("#bookmarks_header").classList.remove("focused");
        this.bookmarkPage.classList.remove("show");
        this.bookmarkPage.classList.add("hide");
    },
    _bookmarkShow: function() {
        document.querySelector("#bookmarks_header").classList.add("focused");
        this.bookmarkPage.classList.remove("hide");
        this.bookmarkPage.classList.add("show");
    },
    _historyHide: function() {
        document.querySelector("#history_header").classList.remove("focused");
        this.historyPage.classList.remove("show");
        this.historyPage.classList.add("hide");
    },
    _historyShow: function() {
        document.querySelector("#history_header").classList.add("focused");
        if (document.querySelectorAll("#history_date .focused").length == 0) {
            document.querySelector("#history_date li").classList.add("focused");
        }
        if (document.querySelectorAll("#history_date .focus").length > 0) {
            document.querySelector("#history_date li").classList.remove("focus");
        }
        this.historyPage.classList.remove("hide");
        this.historyPage.classList.add("show");
    },
    moveBookmarkPage: function() {
        this._historyHide();
        this._bookmarkShow();
        this.state = HomePageManager.states.BOOKMARK_PAGE;
    },
    moveHistoryPage: function() {
        this._bookmarkHide();
        this._historyShow();
        this.state = HomePageManager.states.HISTORY_PAGE;
    }
}

var ScrollBarManager = {
    init: function() {
        this.speedDialScrollBar = new ScrollBar(document.querySelector(".speeddial_scrollBar"));
        this.historyScrollBar = new ScrollBar(document.querySelector(".history_scrollBar"));
        this.speedDialScrollBar.hide();
        this.historyScrollBar.hide();
    }
}

function ScrollBar(scrollBar) {
    this.scrollBarStep = 0;
    this.scrollBar = scrollBar;
    this.scrollBox = scrollBar.parentNode;
    var boxRange = document.defaultView.getComputedStyle(this.scrollBox, "").getPropertyValue("height");
    this.scrollRange = parseInt(boxRange.substring(0, boxRange.length - 2));
    var barHeight = document.defaultView.getComputedStyle(this.scrollBar, "").getPropertyValue("height");
    this.scrollBarHeight = parseInt(barHeight.substring(0, barHeight.length - 2));

    this.show = function() {
        this.scrollBox.classList.remove("hide");
    }

    this.hide = function() {
        this.scrollBox.classList.add("hide");
    }

    this.moveScrollBar = function(index) {
        this.scrollBar.style.top = this.scrollBarStep * index + "px";
    }

    this.changeBarHeight = function(index) {
        this.scrollBarHeight = 50;
        this.scrollBarStep = (this.scrollRange - 50) / (index - 1);
        if(this.scrollBarStep > 50){
            this.scrollBarHeight = this.scrollRange / index;
            this.scrollBarStep = this.scrollBarHeight;
        }
        this.scrollBar.style.height = this.scrollBarHeight + "px";
    }
}

var Recommmend = {

}
var PlayControl = {
    _fullscreen_player:null,
    _button_array : null,
    _browserLoading: null,
    _time_infos: null,
    _progress_background: null,
    _progressbar:null,
    _hideTimer: null,
    _play_pause:false,
    _progress_background_width: 1920 * 0.7,
    _progress_left_coordinate: 1920 * 0.3 * 0.5,

    init:function(){
        this._button_array = document.querySelectorAll(".browser-player-controller-button");
        this._button_array[0].onclick = this._backward;
        this._button_array[1].onclick = this._playPauseControl;
        this._button_array[2].onclick = this._forward;
        this._button_array[3].onclick = this._exitFullScreen;
        this._removeAllFocus();
        this._button_array[1].classList.add("focus");
        this._fullscreen_player = document.querySelector(".browser-fullscreen-player");
        this._browserLoading = document.querySelector(".hisenseBrowserLoading");
        this._time_infos = document.querySelectorAll(".browser-time-info");
        this._progress_background = document.querySelector(".browser-progress-background");
        this._progressbar = document.querySelector(".browser-progress-info");
        this._fullscreen_player.onmousemove=function(e){
            if (KeyboardHandler.getState()==KeyboardHandler.states.FULL_SCREEN_VIDEO_PLAY) {
                PlayControl._playerbarAutoHide();
            }
        }
        this._progress_background.onclick = function(e) {
            var percentage = parseInt(100 * (e.clientX - PlayControl._progress_left_coordinate) / PlayControl._progress_background_width);
            console.log("percentage : " + percentage);
            sendCommand("setVideoPercentage", percentage);
        }
        this._progressbar.onclick = function(e) {
            var percentage = parseInt(100 * (e.clientX - PlayControl._progress_left_coordinate) / PlayControl._progress_background_width);
            console.log("percentage : " + percentage);
            sendCommand("setVideoPercentage", percentage);
        }
        this._playControlDisplayNone();
    },
    _show: function() {
        //console.log("PlayControl...show");
        this._fullscreen_player.classList.remove("browser-player-display-none");
        this._fullscreen_player.classList.remove("browser-player-hide");
        this._fullscreen_player.classList.add("browser-player-show");
    },
    _hide:function(){
        this._fullscreen_player.classList.remove("browser-player-display-none");
        this._fullscreen_player.classList.remove("browser-player-show");
        this._fullscreen_player.classList.add("browser-player-hide");
    },
    _hideLoading: function() {
        this._browserLoading.classList.remove("browser-player-display-none");
        this._browserLoading.classList.remove("browser-player-show");
        this._browserLoading.classList.add("browser-player-hide");
    },
    _removeAllFocus: function() {
        for (var i = this._button_array.length - 1; i >= 0; i--) {
            this._button_array[i].classList.remove("focus");
        };
    },
    _backward: function(){
        PlayControl._removeAllFocus();
        PlayControl._button_array[0].classList.add("focus");
        sendCommand("video_backward");
    },
    _playPauseControl:function(){
        PlayControl._removeAllFocus();
        PlayControl._button_array[1].classList.add("focus");
        sendCommand("video_play_pause");
    },
    _forward:function(){
        PlayControl._removeAllFocus();
        PlayControl._button_array[2].classList.add("focus");
        sendCommand("video_forward");
    },
    _stop:function(){
        PlayControl._removeAllFocus();
        PlayControl._button_array[3].classList.add("focus");
        sendCommand("video_stop");
        PlayControl._progressbar.style.width = "0%";
        PlayControl._playControlDisplayNone();
        PlayControl._removeAllFocus();
        if (PlayControl._hideTimer != null) {
            window.clearTimeout(PlayControl._hideTimer);
        }
        KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
        NativeApi.blurFocus();
        NativeApi.gainFocus();
    },
    _exitFullScreen:function(){
        PlayControl._removeAllFocus();
        PlayControl._button_array[3].classList.add("focus");
        //sendCommand("video_play_pause");
        PlayControl._progressbar.style.width = "0%";
        PlayControl._playControlDisplayNone();
        PlayControl._removeAllFocus();
        if (PlayControl._hideTimer != null) {
            window.clearTimeout(PlayControl._hideTimer);
        }
        NativeApi.exitFullScreen();
        KeyboardHandler.setState(KeyboardHandler.states.VIRTUAL_MOUSE_NAVIGATION);
        NativeApi.blurFocus();
        NativeApi.gainFocus();
        //document.webkitExitFullscreen();
    },
    _playerbarAutoHide: function() {
        this._show();
        if (this._hideTimer != null) {
            window.clearTimeout(this._hideTimer);
        }
        if (!this._play_pause) {
            this._hideTimer = setTimeout(function() {
                PlayControl._hide();
            }, 10000);
        }
    },
    _playControlDisplayNone:function(){
        this._fullscreen_player.classList.remove("browser-player-hide");
        this._fullscreen_player.classList.remove("browser-player-show");
        this._fullscreen_player.classList.add("browser-player-display-none");
        this._browserLoading.classList.remove("browser-player-hide");
        this._browserLoading.classList.remove("browser-player-show");
        this._browserLoading.classList.add("browser-player-display-none");
    }
}
var TTSEngine = {
    _ttsUtterance:null,
    init:function(){
        this._ttsUtterance = new SpeechSynthesisUtterance
    },
    setText:function(text){
        this._ttsUtterance.text = text;
    },
    speak:function(){
        if(this._ttsUtterance.text != ""){
            window.speechSynthesis.cancel(this._ttsUtterance);
            window.speechSynthesis.speak(this._ttsUtterance);
            this._ttsUtterance.text = "";
        }
    }
}
var UrlAdvice = {
    _addurl:null,
    _addtitle:null,
    change:function(url,title){
        if(url.indexOf("http://") > 0 || url.indexOf("https://") > 0){
            if( url.charAt(url.length - 1) != "/"){
               url = url + "/";
            }
        }else{
            if( url.charAt(url.length - 1) != "/"){
                   url = "http://" + url + "/";
            }else{
                url = "http://" + url;
            }
            }
        this._addurl = url;
        this._addtitle = title;
    },
    add:function(){
        SpeedDial.add(this._addurl,this._addtitle,"");
    }
}

var tabIndex = 0;
var TabsLine = {
    beforeselecttab:null,
    init: function() {
    },

    createNewtab: function(id,url) {
    	if (tabIndex >= SETTINGS.MAX_TABS) {
    		console.log("no more tabs");
    	}
    	else{
    		console.log("create new tab");
        	var bt=document.createElement("button");
        	var spanid = "span" + tabIndex;
        	var imgid  = "img" + tabIndex;
        	bt.innerHTML ="<img id=" + imgid + " class = 'favicon' src = '../../abc.jpg'" + ">" + "<span id = " + spanid + ">" + i18n(url) + "</span>";
        	bt.setAttribute("id", "table-" + tabIndex);
        	bt.setAttribute("tabindex", id);
        	bt.setAttribute("loadstate", 0);
        	bt.setAttribute("data-state", "TAB_NAVIGATION");
        	bt.classList.add("tables_button");
        	tabIndex++;
        	document.getElementById("multi-tabs").style.width = SETTINGS.TAB_WIDTH*(tabIndex);
        	document.getElementById("multi-tabs").appendChild(bt);
        	document.getElementById(imgid).classList.add("hide");
        	if (tabIndex >= SETTINGS.MAX_TABS && !document.getElementById("add_tables_button").classList.contains("disabled") ) {
        		document.getElementById("add_tables_button").classList.add("disabled");
        	}
        	if (tabIndex == SETTINGS.MIN_TABS) {
        		document.getElementById("close_tables_button").classList.add("disabled");
        	}
        	if (tabIndex > SETTINGS.MIN_TABS && document.getElementById("close_tables_button").classList.contains("disabled")) {
        		document.getElementById("close_tables_button").classList.remove("disabled");
        	}
    	}
    },
    removeNewtab: function(tab) {
    	if (tabIndex <= SETTINGS.MIN_TABS) {
    		console.log("Please do not close the last tab");
    	}else
    	{
    		var Multitabs = document.getElementById("multi-tabs");
    		var multitab = document.getElementById("multi-tabs").getElementsByTagName("button");
    		var i ;
    		for (i = 0; i < tabIndex; i++) {
    			if (multitab[i].getAttribute("tabindex") == tab) {
    				Multitabs.removeChild(multitab[i]);
    				break;
    			}
    		}
    		tabIndex--;
    		document.getElementById("multi-tabs").style.width = SETTINGS.TAB_WIDTH*(tabIndex);
    		TabsLine.render();
    		if (document.getElementById("add_tables_button").classList.contains("disabled") && tabIndex < SETTINGS.MAX_TABS) {
    			document.getElementById("add_tables_button").classList.remove("disabled");
    		}
    		if (tabIndex <= SETTINGS.MIN_TABS && !document.getElementById("close_tables_button").classList.contains("disabled")){
    			document.getElementById("close_tables_button").classList.add("disabled");
    		}
    	}
    },

    updateTabTitle: function(id,title){
    	var spanid = "span" + id;
    	var imgid = "img" + id;
    	document.getElementById(spanid).innerHTML = i18n(title);
    	//document.getElementById(imgid).classList.remove("hide");
    },

    setActiveTab: function(id){
        if (this.beforeselecttab != null) {
            this.beforeselecttab.classList.remove("select");
        }
        var multitabs = document.getElementById("multi-tabs").getElementsByTagName("button");
        var i;
        for (i = 0; i < multitabs.length; i++) {
            if (multitabs[i].getAttribute("tabindex") == id) {
                multitabs[i].classList.add("select");
                if (multitabs[i].getAttribute("loadstate") == 0) {
                	document.getElementById("loading_status").classList.add("hide");
                }else if (multitabs[i].getAttribute("loadstate") == 1) {
                	document.getElementById("loading_status").classList.remove("hide");
                }
                this.beforeselecttab = multitabs[i];
                break;
            }
        }
    },

    render: function() {
    	var tabs = document.getElementById("multi-tabs").getElementsByTagName("button");
    	var tabNums = tabs.length;
    	var i;
    	for (i = 0 ; i < tabNums; i++) {
    		tabs[i].setAttribute("id", "table-" + i);
    		tabs[i].setAttribute("tabindex", i);
    		tabs[i].firstElementChild.setAttribute("id","img" + i);
    		tabs[i].firstElementChild.nextElementSibling.setAttribute("id","span" + i);
    	}
    },
};

i18n.init();
Application.init();
ScrollBarManager.init();
KeyboardHandler.init();
MouseHandler.init();
LoadingBar.init();
NativeApi.init();
Sections.init();

Bookmarks.init();
Popups.init();
HomePageManager.init();
SpeedDial.init();
//NativeApi.loadUrl("about:blank");

Tabs.addTab(0, "Home Page");
Tabs.setActiveTab(0);
Tabs.getTab(0).setSecureState(0);

NativeApi.setZoom(1 * 1.5);
PlayControl.init();
TTSEngine.init();

// FOR DEBUG ON DESKTOP
if (NON_INTEGRATED) {
    /*
    Tabs.addTab(1, "about:blank");
    Tabs.setActiveTab(1);
    Tabs.getTab(1).setSecureState(0);
    KeyboardHandler.setState(KeyboardHandler.states.SPEED_DIAL_PAGE_NAVIGATION);
    Sections.renderHistory(0);
    */
}


// TRANSLATIONS FOR LATER USE
if (false) {
    i18n("Select action");
    i18n("Speed Dial Error");
    i18n("Move down to hide address bar");
    i18n("Back");
    i18n("Forward");
    i18n("Reload");
    i18n("Stop");
    i18n("Speed Dial");
    i18n("Menu");
    i18n("Add Link to Speed Dial");
    i18n("Add Link to bookmarks");
    i18n("Webpage address is too long. Please reduce the length of the address.");
    i18n("Change Zoom in Menu > Settings");
    i18n("JavaScript Enabled");
    i18n("JavaScript Disabled");
    i18n("A script on this page is slowing down the browser. Do you want to disable JavaScript for this page?");
    i18n("Cookies Enabled");
    i18n("Cookies Disabled");
    i18n("Clear All Cookies");
    i18n("Restore Default Settings");
    i18n("Untrusted Connection");
    i18n("Unsecured Connection");
    i18n("Secure Connection");
    i18n("First Visited");
    i18n("Edit bookmark");
    i18n("Bookmark Error");
    i18n("Close Tab");
    i18n("Do you want to close the tab \"$title\"?");
    i18n("Show All History");
    i18n("All");
    i18n("Today");
    i18n("Yesterday");
    i18n("Last Week");
    i18n("Last Month");
    i18n("Monday");
    i18n("Tuesday");
    i18n("Wednesday");
    i18n("Thursday");
    i18n("Friday");
    i18n("Saturday");
    i18n("Sunday");
}

Date.prototype.Format = function (fmt) {
    var o = {
        "M+": this.getMonth() + 1, //Month
        "d+": this.getDate(), //day
        "h+": this.getHours(), //hour
        "m+": this.getMinutes(), //minute
        "s+": this.getSeconds(), //second
        "q+": Math.floor((this.getMonth() + 3) / 3), //quarter
        "S": this.getMilliseconds() //millisecond
    };
    if (/(y+)/.test(fmt)) {
        fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    }
    for (var k in o) {
        if (new RegExp("(" + k + ")").test(fmt)) {
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
        }
    }
    return fmt;
}
