# 本リポジトリの説明
TOPPERS([Download – EV3 Platform](http://dev.toppers.jp/trac_user/ev3pf/wiki/Download))よりダウンロードしたZIPファイルを展開した。  
本リポジトリは、個人的なCIのテスト環境として利用する目的の為、不具合かどうか利用者が見極める事

# 事前準備
## 1. Ubuntu 14.04.04をインストールする
[Mirrors : Ubuntu](https://launchpad.net/ubuntu/+cdmirrors)から、ISOイメージをダウンロードし、インストールする

## 2. Ubuntuに開発環境を構築する
1. OpenSSHのインストール : `sudo apt-get install openssh-server`
1. Gitのインストール : http://qiita.com/masanori1102/items/4d43d4a0c7211c3061ac
1. 開発環境の構築 : http://dev.toppers.jp/trac_user/ev3pf/wiki/DevEnvLinux
1. DoxygenとGraphvizのインストール : `sudo apt-get install doxygen doxygen-gui graphviz`
1. GitHubのアカウントを登録するか、SSH認証をおこなう

# スクリプトの実行
プロジェクトのルートで以下のコマンドを実行する。

## 1. 環境構築&バイナリファイルの生成
開発環境を作成し、後述の対象ディレクトリに対して、appビルドとimgビルドをおこなう。

|項目|値|
| --- |---|---|
|実行するコマンド|[./scripts/createEnv_and_makeAll.sh](scripts/createEnv_and_makeAll.sh)|
|対象ディレクトリ|[hrp2/sdk/workspace](hrp2/sdk/workspace)以下|
|出力先ディレクトリ|[hrp2/sdk](hrp2/sdk)`/bin/{app | img}/{ディレクトリ名}`以下(にバイナリファイルとビルドログ、オブジェクトが出力される)|

## 2. Doxygenの更新とmasterブランチのマージ
対象ディレクトリのソースコードを基に、DoxygenとGraphvizのファイルを生成する

|項目|値|
| --- |---|---|
|実行するコマンド|[./scripts/update_merge_Doxyfiles.sh](scripts/update_merge_Doxyfiles.sh)|
|対象ディレクトリ|[hrp2/sdk/workspace](hrp2/sdk/workspace)以下|
|出力先ディレクトリ|[hrp2/sdk/html](hrp2/sdk/html)|
