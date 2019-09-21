# Prophet & time series forcasting

## Introduction

#### To Fix Some Hard Facts

- The result is that the demand for high quality forecasts often far outstrips the pace at which they can be produced.
- Hard-to-tune parameters in traditional models.
- Completely automatic forecasting techniques can be brittle and they are often too inflexible to incorporate useful assumptions or heuristics.
- Analysts who can produce high quality forecasts are quite rare because forecasting is a specialized data science skill requiring substantial experience.

#### Features of Bussiness Time Series

- Cycles: weekly and yearly cycles.(seasonal effects)

## Overview

#### Model

The prophet model is a decomposable additive model which consists of 3 main components: trend($g(t)$ ), seasonality($s(t)$ ) and holidays($h(t)$ ).
$$
y(t) = g(t)+s(t)+h(t)+\varepsilon_t
$$

> Here $g(t)$ is the trend funtion which models non-periodic changes in the value of the time series, $s(t) $ represents periodic  changes(*e.g.,* weekly and yearly seasonality), and $h(t)$ represents the effects of holidays which occur on potentially irregular schedules over one or more days. The error term $\varepsilon_t$ represents any idiosyncratic changes which are not accommodated by the model. Later we will make the parametric assumption that $\varepsilon_t$ is normally distributed. (As you see, time is the only regressor.)

> This specification is similar to a generalized additive model(GAM)(Hastie & Tibshirani 1987), a class of regression models with potentially non-linear smoothers applied to the
> regressors. The GAM formulation has the advantage that it decomposes easily and accommodates new components as necessary, for instance when a new source of seasonality is identified.

#### Advantages

While we give up some important inferential advantages of using a generative model such as an ARIMA, this formulation provides a number of practical advantages:

- **Flexibility**: Seasonality with multiple periods can be easily accommodated and the user can make different assumption about the *trend*($g(t)$).

- Unlike with ARIMA models, the measurements do not need to be regularly spaced, and **we do not need to interpolate missing values** *e.g.* from removing outliers.
- **Fast** to fit.
- The forecasting model has easily **interpretable** **parameters** that can be changed by the analyst to impose assumptions on the forecast. Moreover, analysts typically do have experience with regression and are easily able to extend the model to include new components.

#### Where Prophet shines

Not all forecasting problems can be solved by the same procedure. Prophet is optimized for the business forecast tasks we have encountered at Facebook, which typically have any of the following characteristics:

- hourly, daily, or weekly observations with at least a few months (preferably a year) of history.
- strong multiple “human-scale” seasonalities: day of week and time of year.
- important holidays that occur at irregular intervals that are known in advance (e.g. the Super Bowl).
- a reasonable number of missing observations or large outliers.
- historical trend changes, for instance due to product launches or logging changes.
- trends that are non-linear growth curves, where a trend hits a natural limit or saturates.

## Inside the model

#### The Trend Model

In facebook opensource tool `fbprophet`, 2 trend models are implemented:

- A saturating growth model;
- A piecewise linear model;

In this article, we talk about the saturating growth model only.

##### Nonlinear, Saturating Growth(饱和增长)

This is typically modeled using the logistic growth model:
$$
g(t) = \frac{C}{1+\exp\left(-k(t-m)\right)}
$$

> With C the carrying capacity, k the growth rate and m an offset parameter.

But in most cases(not only in facebook's case), C & k is not constant. We thus replace the fixed capacity C with a time-varying capacity C(t). As to the growth rate k, We incorporate trend changes in the growth model by explicitly defining changepoints where the growth rate is allowed to change. 

For time points $s_j, j=1,\cdots,S$, the growth rate is $k+\sum_{j:t>s_j}\delta_j$ , where $k$ represents the base growth rate and $\delta_j$ represents the the change in rate occures at time $s_j$. This is represented more cleanly by defining a vector $\mathbf{a}(t) ∈ \{0, 1\}^S$ and ${\mathbf{\delta}=\{\delta_1,\cdots,\delta_S\}}$ such that
$$
a_j(t) = \left\{
\begin{aligned}
1, &{\ \ \  \ \ \rm if}\ t\ge s_j,\\
0, &\ \ \  \ \ otherwise.
\end{aligned}
\right.
$$
The growth rate is then $k+\mathbf{a}(t)^T\mathbf{\delta}$.

When the rate $k$ is adjusted, the offset parameter $m$ must also be adjusted to connect the endpoints of the segments. The correct adjustment at changepoint j is easily computed as

![img](https://s2.ax1x.com/2019/01/21/kPX2gH.png)



#### Seasonality

Fourier series.

#### Holidays and Events

Holidays and events provide large, somewhat predictable shocks to many business time series and often do not follow a periodic pattern, so their effects are not well modeled by a smooth cycle.(*e.g.* Chinese spring festival happends according to lunar calendar while double eleven shopping day happends according to solar calendar.)

This can be fixed by a provided holiday list. Incorporating this list of holidays into the model is made straightforward by assuming that the effects of holidays are independent. For each holiday $i$, let $D_i$ be the set of past and future dates for that holiday.
$$
k=\{k_i\}\\
% :1\ {\rm if}\ t\in D_i\ {\rm otherwise}\ 0
Z(t) = [1(t\in D_1),\cdots,1(t\in D_L)]\\
h(t)=Z(t)k
$$

#### Model fitting

For model fitting we use Stan’s L-BFGS to find a maximum a posteriori estimate.

## How to use

![img](https://research.fb.com/wp-content/uploads/2017/02/image-2.png?w=884)

#### core

At its core, the Prophet procedure is an [additive regression model](https://en.wikipedia.org/wiki/Additive_model) with four main components:

- A piecewise linear or logistic growth curve trend. Prophet automatically detects changes in trends by selecting changepoints from the data.
- A yearly seasonal component modeled using Fourier series.
- A weekly seasonal component using dummy variables.
- A user-provided list of important holidays.

#### Python API

##### download

```python
conda install fbprophet
```

##### Guide

> Prophet follows the sklearn model API. We create an instance of the Prophet class and then call its fit and predict methods.

> The input to Prophet is always a dataframe with two columns: `ds` and `y`. The `ds`(datestamp) column should be of a format expected by Pandas, ideally YYYY-MM-DD for a date or YYYY-MM-DD HH:MM:SS for a timestamp. The `y` column must be numeric, and represents the measurement we wish to forecast.

[Quick Start](https://facebook.github.io/prophet/docs/quick_start.html)

## Reference

[1] Taylor SJ, Letham B. 2017. Forecasting at scale. PeerJ Preprints 5:e3190v2<https://doi.org/10.7287/peerj.preprints.3190v2>

[2] https://research.fb.com/prophet-forecasting-at-scale/