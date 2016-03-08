# 目的

誰でもEV3のステートマシンにステートを追加できるようにする.

# 手順

1. ステートのクラスを作る
1. ステートを識別するIDを決める
1. IDと, ステートのインスタンスを紐付ける
1. 状態遷移表に, 遷移を定義する

以下, それぞれの詳細説明.

## ステートのクラスを作る

1. 下にあるコードをコピペ
1. コード中に日本語で書いてある部分を埋める (埋めないとコンパイルできないようにしてある.)
1. StateMachine/Statesフォルダーに保存する
1. プロジェクトに追加する

```C#
using System;
using ETRobocon.EV3;

namespace ETRobocon.StateMachine
{
	public class -ステート名を入れてね-State : State
	{

		-必要に応じてプライベートフィールドや定数を追加-
		-メソッドは追加しないでほしい(追加する必要ないはず)-

		public -ステート名を入れてね-State(EV3body body, -必要に応じて引数を追加-) : base(body, -実行動作の繰り返し間隔[ms]を入れてね-)
		{
			-コンストラクタを実装する-
		}

		public override void Enter()
		{
			-Enter()を実装する-
		}

		public override void Do()
		{
			-Do()を実装する-
		}

		public override void Exit()
		{
			-Exit()を実装する-
		}

		public override TriggerID JudgeTransition()
		{
			if (-優先度が高いトリガー条件-)
			{
				return TriggerID.-返すトリガー-;
			}
			else if (-優先度が普通のトリガー条件-)
			{
				return TriggerID.-返すトリガー-;
			}
			else if (-優先度が低いトリガー条件-)
			{
				return TriggerID.-返すトリガー-;
			}

			return TriggerID.NoTrigger;
		}
	}
}
```

## ステートを識別するIDを決める

`enum StateID` に, 値を追加.

10文字以内程度のできるだけ短い識別子を推奨します.

例 : RCurve1を追加してみる
```C#
public enum StateID
	{
		// 走行準備
		Ready,

		// 走行
		Straight1,	///< 最初の直線区間
追加! →	RCurve1,	///< 次にくる右カーブ

		// ルックアップゲート用

		// フィギュアL用

		// その他
		Complete,	///< 完走 (走行停止)

		NumOfState
	}
```

## IDと, ステートのインスタンスを紐付ける

StateMachineクラスのコンストラクタ内で, インスタンスを作成し, 紐付ける.

_`enum StateID` の定義順序と一致させること_

例 : RightCurveWithLineTraceStateを, RCurve1として登録する
```C#
_states = new State[(int)StateID.NumOfState]
			{
				new ReadyState(_body),	// Ready
				new StraightWithLineTraceState(_body),	// Straight1
追加! →			new RightCurveWithLineTraceState(_body, 必要に応じて引数を追加),	// RCurve1
				new CompleteState(_body)	// Complete
			};
```

## 状態遷移表に, 遷移を定義する

_`enum StateID` の定義順序と一致させること_

あるステートからあるステートへの遷移に特有の処理があれば, StateMachineクラスに定義し, `Nop`と置き換える.

そのステートからどのステートへ移動するか, だけでなく, どのステートから来るかの定義も忘れずに.

例 : RCurve1として登録する
```C#
TransitionMatrix = new Transition?[(int)StateID.NumOfState, (int)TriggerID.NumOfTrigger]
			{
				// TouchSensor,								RunCommand,								StopCommand

				// 走行準備
				{ new Transition(StateID.Straight1, Nop),	new Transition(StateID.Straight1, Nop),	null },

				// ゴールまで走行
				                            ↓ RCurve1へ来る遷移を定義するのをお忘れなく
				{ new Transition(StateID.RCurve1, Nop),		null,									new Transition(StateID.Complete, Nop) },
追加! →			{ new Transition(StateID.Complete, Nop),	null,									new Transition(StateID.Complete, Nop) },

				// ルックアップゲート用

				// フィギュアL用

				// ...

				// その他
				{ null,										null,									null }
			};
```

以上
