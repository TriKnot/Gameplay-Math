#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasingType.h"
#include "EasingUtils.generated.h"

/**
 * A library of easing functions that can all be passed a normalized float.
 * Grabbed from https://easings.net/
 */
UCLASS()
class GAMEPLAYMATHV2_API UEasingUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static float LinearEase(const float T)
	{
		return T;
	}
	
	static float Sine_In(const float T)
	{
		return 1.f - FMath::Cos((T * PI) / 2);
	}

	static float Sine_Out(const float T)
	{
	    return FMath::Sin((T * PI) / 2);
	}

	static float Sine_InOut(const float T)
	{
	    return -(FMath::Cos(PI * T) - 1) / 2;
	}

	static float Quad_In(const float T)
	{
	    return T * T;
	}

	static float Quad_Out(const float T)
	{
	    return 1 - (1 - T) * (1 - T);
	}

	static float Quad_InOut(const float T)
	{
	    return T < 0.5 ? 2 * T * T : 1 - FMath::Pow(-2 * T + 2, 2) / 2;
	}

	static float Cubic_In(const float T)
	{
	    return T * T * T;
	}

	static float Cubic_Out(const float T)
	{
	    return 1 - FMath::Pow(1 - T, 3);
	}

	static float Cubic_InOut(const float T)
	{
	    return T < 0.5 ? 4 * T * T * T : 1 - FMath::Pow(-2 * T + 2, 3) / 2;
	}

	static float Quart_In(const float T)
	{
	    return T * T * T * T;
	}

	static float Quart_Out(const float T)
	{
	    return 1 - FMath::Pow(1 - T, 4);
	}

	static float Quart_InOut(const float T)
	{
	    return T < 0.5 ? 8 * T * T * T * T : 1 - FMath::Pow(-2 * T + 2, 4) / 2;
	}

	static float Quint_In(const float T)
	{
	    return T * T * T * T * T;
	}

	static float Quint_Out(const float T)
	{
	    return 1 - FMath::Pow(1 - T, 5);
	}

	static float Quint_InOut(const float T)
	{
	    return T < 0.5 ? 16 * T * T * T * T * T : 1 - FMath::Pow(-2 * T + 2, 5) / 2;
	}

	static float Expo_In(const float T)
	{
	    return T == 0 ? 0 : FMath::Pow(2, 10 * T - 10);
	}

	static float Expo_Out(const float T)
	{
	    return T == 1 ? 1 : 1 - FMath::Pow(2, -10 * T);
	}

	static float Expo_InOut(const float T)
	{
	    return T == 0 ? 0 : T == 1 ? 1 : T < 0.5 ? FMath::Pow(2, 20 * T - 10) / 2 : (2 - FMath::Pow(2, -20 * T + 10)) / 2;
	}

	static float Circ_In(const float T)
	{
	    return 1 - FMath::Sqrt(1 - FMath::Pow(T, 2));
	}

	static float Circ_Out(const float T)
	{
	    return FMath::Sqrt(1 - FMath::Pow(T - 1, 2));
	}

	static float Circ_InOut(const float T)
	{
	    return T < 0.5f ? (1 - FMath::Sqrt(1 - FMath::Pow(2 * T, 2))) / 2 : (FMath::Sqrt(1 - FMath::Pow(-2 * T + 2, 2)) + 1) / 2;
	}

	static float Back_In(const float T)
	{
	    constexpr float C1 = 1.70158f;
	    constexpr float C3 = C1 + 1.f;

	    return C3 * T * T * T - C1 * T * T;
	}

	static float Back_Out(const float T)
	{
	    constexpr float C1 = 1.70158f;
	    constexpr float C3 = C1 + 1;

	    return 1 + C3 * FMath::Pow(T - 1, 3) + C1 * FMath::Pow(T - 1, 2);
	}

	static float Back_InOut(const float T)
	{
	    constexpr float C1 = 1.70158f;
	    constexpr float C2 = C1 * 1.525f;

	    return T < 0.5f ? (FMath::Pow(2 * T, 2) * ((C2 + 1) * 2 * T - C2)) / 2 : (FMath::Pow(2 * T - 2, 2) * ((C2 + 1) * (T * 2 - 2) + C2) + 2) / 2;
	}

	static float Elastic_In(const float T)
	{
	    constexpr float C4 = (2 * PI) / 3;
	    return T == 0 
	        ? 0 
	        : T == 1 
	        ? 1 
	        : -FMath::Pow(2, 10 * T - 10) * FMath::Sin((T * 10 - 10.75f) * C4);
	}

	static float Elastic_Out(const float T)
	{
	    constexpr float C4 = (2 * PI) / 3;

	    return T == 0 
	        ? 0 
	        : T == 1 
	        ? 1 
	        : FMath::Pow(2, -10 * T) * FMath::Sin((T * 10 - 0.75f) * C4) + 1;
	}

	static float Elastic_InOut(const float T)
	{
	    constexpr float C5 = (2 * PI) / 4.5f;

	    return T == 0 ? 0 : T == 1 ? 1 : T < 0.5f ? -(FMath::Pow(2, 20 * T - 10) * FMath::Sin((20 * T - 11.125f) * C5)) / 2
	      : (FMath::Pow(2, -20 * T + 10) * FMath::Sin((20 * T - 11.125f) * C5)) / 2 + 1;
	}

	static float Bounce_In(const float T)
	{
	    return 1 - Bounce_Out(1 - T);
	}

	static float Bounce_Out(float T)
	{
	    constexpr float N1 = 7.5625f;
	    constexpr float D1 = 2.75f;

	    if (T < 1 / D1)
	    {
    		return N1 * T * T;
	    }
			
	    if (T < 2 / D1)
	    {
			return N1 * (T -= 1.5f / D1) * T + 0.75f;
	    }
			
	    if (T < 2.5 / D1)
	    {
			return N1 * (T -= 2.25f / D1) * T + 0.9375f;
	    }

		return N1 * (T -= 2.625f / D1) * T + 0.984375f;
	}

	static float Bounce_InOut(const float T)
	{
	    return T < 0.5f ? (1 - Bounce_Out(1 - 2 * T)) / 2 : (1 + Bounce_Out(2 * T - 1)) / 2;
	}

	UFUNCTION(BlueprintCallable, Category = "Easing")
	static float GetEaseValue(const EEasingType EasingType, const float T)
	{
	    switch(EasingType) 
	    {
	    	case Linear: return LinearEase(T);
	    	
	        case SineIn: return Sine_In(T);
	        case SineOut: return Sine_Out(T);
	        case SineInOut: return Sine_Out(T);
	        
	        case QuadIn: return Quad_In(T);
	        case QuadOut: return Quad_Out(T);
	        case QuadInOut: return Quad_InOut(T);

	        case CubeIn: return Cubic_In(T);
	        case CubeOut: return Cubic_Out(T);
	        case CubeInOut: return Cubic_InOut(T);

	        case QuartIn: return Quart_In(T);
	        case QuartOut: return Quart_Out(T);
	        case QuartInOut: return Quart_InOut(T);

	        case QuintIn: return Quint_In(T);
	        case QuintOut: return Quint_Out(T);
	        case QuintInOut: return Quint_InOut(T);

	        case ExponentialIn: return Expo_In(T);
	        case ExponentialOut: return Expo_Out(T);
	        case ExponentialInOut: return Expo_InOut(T);

	        case CircularIn: return Circ_In(T);
	        case CircularOut: return Circ_Out(T);
	        case CircularInOut: return Circ_InOut(T);

	        case BackBounceIn: return Back_In(T);
	        case BackBounceOut: return Back_Out(T);
	        case BackBounceInOut: return Back_InOut(T);

	        case ElasticIn: return Elastic_In(T);
	        case ElasticOut: return Elastic_Out(T);
	        case ElasticInOut: return Elastic_InOut(T);

	        case BounceIn: return Bounce_In(T);
	        case BounceOut: return Bounce_Out(T);
	        case BounceInOut: return Bounce_InOut(T);

	        default: return T;
	    }
	}
};
