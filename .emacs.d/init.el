;;----------------------外观--------------------
;使用solarized主题
(add-to-list 'custom-theme-load-path "~/.emacs.d/elpa/color-theme-solarized-20160626.743")
(load-theme 'solarized t)
(set-frame-parameter nil 'background-mode 'dark)
(enable-theme 'solarized)

;; 去掉工具栏
(tool-bar-mode -1)
;; 去掉mode-line中的行号指示
(line-number-mode -1)
;;去掉菜单栏
(menu-bar-mode -1)
;; 去掉滚动栏
(scroll-bar-mode -1)
;;关闭 Emacs 的启动画面。
(setq inhibit-startup-message t)

;; 显示时间
(display-time-mode -1)
;; 24小时模式
(setq display-time-24hr-format t)
;; 显示日期
(setq display-time-day-and-date t)
;; 显示电池状态
(setq display-battery-mode t)
;; 电池格式
;; ;;(battery-mode-line-format "[%b%p%%,%t]")


;--------------------操作时颜色设置--------------------

;; 选择区域颜色
(set-face-foreground 'region "slate blue")
;; 选择区域文字颜色
(set-face-background 'region "cyan")

;; (set-face-foreground 'secondary-selection "skyblue")
;; (set-face-background 'secondary-selection "darkblue")

;;访问同名文件时,buffer前显示父目录以区别
(setq uniquify-buffer-name-style 'forward)

;;显示括号
(show-paren-mode t)
;;
(setq show-paren-style 'parenthesis)

;;鼠标规避
(mouse-avoidance-mode 'exile)

;; 在window右边的fringe中显示上下箭头和边界标志
(setq-default indicate-buffer-boundaries 'right)
;; 标注buffer中未使用的行
(setq-default indicate-empty-lines t)
;; 高亮当前行
(global-hl-line-mode t)
;; 当出错时,闪屏
(setq visible-bell 1)
;; 实时显示输入快捷键
(setq echo-keystrokes 0.01)

;;----------光标----------

;; 光标不停的闪烁
(setq blink-cursor-blinks 0)
;; 光标颜色为紫红色
(set-face-background 'cursor "Orchid")
;; 让光标覆盖特殊字符的宽度,如tab
(setq x-stretch-cursor t)

;;-------------------- 滚屏设置--------------------

;; 每次自动滚屏一行
(setq scroll-step 1)
;; 当光标距离上下边界3行时,开始自动滚屏
(setq scroll-margin 2)
;; 滚屏后光标不要位于屏幕中间
(setq scroll-conservatively 500)
;; 翻页时保留的重叠行数
(setq next-screen-context-lines 3)
;; 保持光标位置不动
(setq scroll-preserve-screen-position t)
;;左右滚屏时的边距
(setq hscroll-margin 2)
;; 左右滚屏时不聚焦到中心
(setq hscroll-step 1)

;;-----------------------------------------

;; 设置自动换行的列数
(setq current-fill-column 100)

;; 用"C-SPC“代替"C-u-SPC"来遍历标记环
(setq set-mark-command-repeat-pop 1)

;;--------------------Minibuffer--------------------

;; 自动调整minibuffer大小
(setq resize-mini-windows t)
;; 打开新文件或新buffer时无需确认
;(setq confirm-nonexistent-file-or-buffer 'after-completion)
;; "C-|"切换到最近的buffer
(global-set-key (kbd "M-o")
		#'(lambda ()
		    (interactive)
		    (switch-to-buffer (other-buffer (current-buffer)))))

;;--------------------地理位置信息--------------------

(setq calendar-latitude 34) ;; 纬度
(setq calendar-longitude 109) ;; 经度
(setq calendar-location-name "Xi'an") ;; 地名西安市

;;--------------------操作设置--------------------

;; 默认采用 text-mode
(setq-default major-mode 'text-mode)
;; 以 y/n代表 yes/no
(fset 'yes-or-no-p 'y-or-n-p)
;; 不自动在文档尾添加空白行
;(setq require-final-newline nil)
;; 剪切read-only buffer时不再报错
(setq kill-read-only-ok 1)
;; 若连续的剪切内容一样的话,只在kill-ring中保存一次
(setq kill-do-not-save-duplicates 1)
;; 选中区域后输入文本将删除该区域内容
(delete-selection-mode 1)
;; 在emacs kill命令前,将将剪切版的内容提前保存到kill-ring,防止丢失
(setq save-interprogram-paste-before-kill 1)
;; 使用寄存器时,使提示窗口延迟2秒响应
(setq register-preview-delay 2)
;; 使删除等命令在未标记时不起作用,防止误删 
(setq mark-even-if-inactive nil)
;; 每次字体缩放的倍数
(setq text-scale-mode-step 1.1)
;; 即时高亮buffer中匹配搜索的内容,无延迟
(setq lazy-highlight-initial-delay 0)
;; 搜索的同时允许翻页
(setq isearch-allow-scroll t)
;; 第一次按tab缩进,若已经缩进则补全当前光标处
(setq tab-always-indent 'complete)


;;--------------------键绑定--------------------
;; 一次滚屏一行

(global-set-key (kbd "M-e") 'scroll-up-line)
(global-set-key (kbd "M-a") 'scroll-down-line)
;; 输入','后自动加空格
(global-set-key (kbd ",")
                #'(lambda ()
                    (interactive)
                    (insert ", ")))
;; 用"C-w"向回删除一个单词
;(global-set-key "\C-w" 'backward-kill-word)
;; 用"C-x-k"删除区域
;(global-set-key "\M-k" 'kill-region)
;; 用"M-k"删除一整行
(global-set-key "\M-k" 'kill-whole-line)

(global-set-key (kbd "C-\\") 'eval-last-sexp)
;; f5 为刷新.emacs设置
(global-set-key [f5] 'eval-buffer)
(global-set-key (kbd "M-<f12>") 'revert-buffer)
(global-set-key [f8] 'list-packages)
(global-set-key [f6] 'toggle-truncate-lines)
(global-set-key [f12] 'replace-string)
;(setq-default compile-command "make")
;; f9 为调试
(global-set-key [f9] 'gdb)

;; 代码折叠
(global-set-key (kbd  "C-M-[") 'hs-show-block)
(global-set-key (kbd  "C-M-]") 'hs-hide-block)
(global-set-key (kbd  "C-M-{") 'hs-show-all)
(global-set-key (kbd  "C-M-}") 'hs-hide-all)

;; 调用man
(global-set-key (kbd  "C-c C-m") 'man)

;; 'C-_'为undo-only
(global-set-key "\C-_" 'undo-only)
;; 'M-='显示buffer信息
(global-set-key (kbd  "M-=") 'count-words)

(global-set-key (kbd "RET") 'newline-and-indent)

;(global-set-key (kbd "<print>") 'keyboard-quit)



;;--------------------Projectile--------------------

;(projectile-global-mode t)

;;--------------------sr-speedbar-------------------- 

;(setq speedbar-use-images nil)		


;;--------------------shell-mode--------------------
;; f2 为 shell
(global-set-key [f2] 'shell)
;;总是打开新的shell
(defadvice shell (around always-new-shell)
  (let ((buffer (generate-new-buffer-name "*shell*")))
    ad-do-it))
(ad-activate 'shell)
;自动去掉shell 输出中的^M字符
(add-hook 'comint-output-filter-functions
                    'comint-strip-ctrl-m)
;(setq comint-buffer-maximum-size )
;;根据comint-buffer-maximum-size变量自动限制shell输出的最大行数(默认为1024)
(add-hook 'comint-output-filter-functions
                    'comint-truncate-buffer)
;; 不在历史记录中保存连续的相同命令
(setq comint-input-ignoredups t)
(setq comint-completion-autolist t)
(setq comint-completion-recexact nil)
;; 设置提示符为只读
(setq comint-prompt-read-only t)
(add-hook 'shell-mode-hook 'ansi-color-for-comint-mode-on t)


;;--------------------Dired--------------------
;; 将删除文件放入回收站,而非直接删除
(setq delete-by-moving-to-trash t)
(setq dired-listing-switches "-Alhk") 
(setq dired-auto-revert-buffer t)
(setq dired-allow-to-change-permissions t) ;允许编辑文件的权限

(add-to-list 'load-path "~/.emacs.d/elpa/dired+-20161022.916")
(require 'dired+)

;; (add-to-list 'load-path "~/.emacs.d/elpa/dired-single-20151230.1758")
;; (require 'dired-single)
;; (dired-toggle-find-file-reuse-dir t)

;; dired-x
(add-hook 'dired-load-hook
	  (lambda ()
	    (load "dired-x")
	    ;; Set dired-x global variables here.  For example:
	    ;; (setq dired-guess-shell-gnutar "gtar")
	    ;; (setq dired-x-hands-off-my-keys nil)
	    ))

;; dired-jump
(autoload 'dired-jump "dired-x"
  "Jump to Dired buffer corresponding to current buffer." t)
;; dired-jump-other-window
(autoload 'dired-jump-other-window "dired-x"
  "Like \\[dired-jump] (dired-jump) but in other window." t)

(define-key global-map "\C-x\C-j" 'dired-jump)
(define-key global-map "\C-x4\C-j" 'dired-jump-other-window)


;;--------------------Flyspell-mode--------------------
;; 用hunspell作为拼写检查的后端
(setq-default ispell-program-name "aspell")
;; 只检查英语单词
(ispell-change-dictionary "american" t)

;;开启session
(desktop-save-mode t)
;; 打开icomplete-mode

;; 大单词模式,下划线视为单词的一部分
(superword-mode t)

;; ;;--------------------smartparens-mode--------------------
(add-to-list 'load-path "~/.emacs.d/elpa/smartparens-20161026.945")
(require 'smartparens)
(smartparens-global-mode 1)

;; (global-set-key (kbd "C-M-a") 'sp-beginning-of-sexp)
;; (global-set-key (kbd "C-M-e") 'sp-end-of-sexp)

;; (Global-set-key (kbd "C-M-f") 'sp-forward-sexp)
;; (global-set-key (kbd "C-M-b") 'sp-backward-sexp)
;; (global-set-key (kbd "C-M-n") 'sp-next-sexp)
;; q(global-set-key (kbd "C-M-p") 'sp-previous-sexp)


;; (global-set-key (kbd "C-M-[") 'sp-backward-unwrap-sexp)
;; (global-set-key (kbd "C-M-]") 'sp-unwrap-sexp)

;; --------------------Multiple-cursors--------------------
;; (require 'multiple-cursors)
;; (global-set-key (kbd "C-S-c C-S-c") 'mc/edit-lines)
;; (global-set-key (kbd "C->") 'mc/mark-next-like-this)
;; (global-set-key (kbd "C-<") 'mc/mark-previous-like-this)
;; (global-set-key (kbd "C-c C-<") 'mc/mark-all-like-this)


;;--------------------备份文件--------------------
;;每输入20个字符就自动保存一次 
(setq auto-save-interval 30);; 不能小于20
(setq auto-save-timeout 60);; 每间隔60s自动保存一次,对于大的buffer,实际时间将超过指定时间.
;;将备份文件保存到"~/.emacs_backups"中
(setq backup-directory-alist (quote 
(
	(".tex" . "~/.emacs_backups/papers")
	(".org" . "~/.emacs_backups/orgs")
	("." . "~/.emacs_backups")
)))

;; 每个备份文件保留多个版本
(setq version-control t)
;; 自动删除中间版本
(setq delete-old-versions t)
;;保存最近的3个版本,和最老的2个版本
(setq kept-old-versions 2)
(setq kept-new-versions 3)


;; 每次设置书签都自动保存
(setq bookmark-save-flag 1)

;; 当文件发生改变时,自动更新对应buffer的内容
(global-auto-revert-mode t)
(setq global-auto-revert-non-file-buffers t)

;;--------------------window-numbering-mode--------------------
(add-to-list 'load-path "~/.emacs.d/elpa/window-numbering-20160809.1110/")
(require 'window-numbering) ;; 用M-0~9来切换窗口
(window-numbering-mode t)

;;---------------------recentf-mode--------------------
(recentf-mode 1)
(global-set-key (kbd"<f7>") 'recentf-open-files) ;;F7显示最近打开的文件列表

;; 自动匹配括号
;(electric-pair-mode t)
;; 设置匹配的括号类型
;(setq electric-pair-pairs '((?\{.?\}) (?\[.?\]) (?\(.?\)) (?\".?\")))

;(smartparens-global-mode t)

;-------------------- winner-mode --------------------

(winner-mode t)
(global-set-key (kbd "C-x 4 u") 'winner-undo)
(global-set-key (kbd "C-x 4 r") 'winner-redo)

;; ;--------------------golden-ratio-mode--------------------
; (add-to-list 'load-path "~/.emacs.d/elpa/golden-ratio-20150526.1200/")
; (require 'golden-ratio)
; (golden-ratio-mode 1)
; (setq golden-ratio-auto-scale t)



;;--------------------avy-mode--------------------
 
(add-to-list 'load-path "~/.emacs.d/elpa/avy-20160814.250")
(require 'avy) 
(avy-setup-default)
(global-set-key (kbd "M-SPC") 'avy-goto-char-2)
;(global-set-key (kbd "C-'") 'avy-goto-word-1)
(global-set-key (kbd "M-g M-g") 'avy-goto-line)
(setq avy-background t) ;;启用灰色遮盖背景
;; 用"C-c-SPC" 代替"M-SPC"
(define-key global-map (kbd "C-c C-SPC") 'just-one-space)

;;--------------------avy-zap-mode--------------------

(add-to-list 'load-path "~/.emacs.d/elpa/avy-zap-20160921.1444")
(require 'avy-zap)
(global-set-key (kbd "M-z") 'avy-zap-up-to-char)
(global-set-key (kbd "M-Z") 'avy-zap-to-char)
 

;;--------------------flycheck-mode--------------------

(add-to-list 'load-path "~/.emacs.d/elpa/flycheck-20161025.551")
(require 'flycheck)

(add-hook 'after-init-hook #'global-flycheck-mode)
(setq flycheck-display-errors-delay 0.1) ;;0延迟显示错误信息

;;启用浮动错误提示
(add-to-list 'load-path "~/.emacs.d/elpa/flycheck-pos-tip-20160323.129")
(require 'flycheck-pos-tip)

(with-eval-after-load 'flycheck
  (flycheck-pos-tip-mode))
;; 持久显示浮动错误信息(1000s以后再消失)
(setq flycheck-pos-tip-timeout 1000)

;;--------------------Company-mode--------------------
(add-hook 'after-init-hook 'global-company-mode)
(setq company-idle-delay 0)
(setq company-minimum-prefix-length 2)
;(setq company-backends (delete 'company-semantic company-backends))
(global-set-key (kbd "<backtab>") 'company-complete)
;(add-to-list 'company-backends 'company-c-headers);自动补全c/c++头文件
;; (add-to-list 'company-c-headers-path-user "/home/pz/code/libs/_h")
;; (add-to-list 'company-c-headers-path-user "/home/pz/code/MASM/")
;; (add-to-list 'company-c-headers-path-user "/home/pz/code/Hash_match/include")

;;--------------------Undo-tree-mode--------------------

(add-to-list 'load-path "~/.emacs.d/elpa/undo-tree-20161012.701/")
(require 'undo-tree)
(global-undo-tree-mode 1)

;;--------------------Auctex--------------------

(setq TeX-auto-save t)
(setq TeX-parse-self t)
(setq-default TeX-master nil)

;;hooks

(add-hook 'LaTeX-mode-hook
	  (lambda()
	    (auto-fill-mode t)
	    (flyspell-mode t)
	    (LaTeX-math-mode t)
	    (abbrev-mode t)
	    (TeX-source-correlate-mode t)
	    (turn-on-reftex)
	    (follow-mode 1)
	    (server-start 1)
	    (Tex-fold-mode 1)
	    
	    )
	  )
(setq Tex-save-query nil)
;; (add-hook 'LaTeX-mode-hook 'auto-fill-mode)
;; (add-hook 'LaTeX-mode-hook 'flyspell-mode)
;; (add-hook 'LaTeX-mode-hook 'LaTeX-math-mode)
;; (add-hook 'LaTeX-mode-hook 'abbrev-mode)
;; (add-hook 'LaTeX-mode-hook 'TeX-source-correlate-mode)
;; (add-hook 'LaTeX-mode-hook 'turn-on-reftex)
;; (add-hook 'LaTeX-mode-hook 'follow-mode)
;; (add-hook 'LaTeX-mode-hook 'server-start)
;; (add-hook 'Latex-mode-hook 'TeX-fold-mode)

(setq reftex-external-file-finders   
'(("tex" . "kpsewhich -format=.tex %f")   
("bib" . "kpsewhich -format=.bib %f")))


(setq TeX-fold-auto t);每次输入宏时,自动折叠
;; (setq TeX-view-program-selection
;;  '((output-pdf "PDF Viewer")))
;; (setq TeX-view-program-list
;;  '(("PDF Viewer" "okular --unique %o#src:%n%b")))

;;--------------------expand-region--------------------
(global-set-key (kbd "M-<return>") 'er/expand-region)

;;--------------------编译--------------------
;; f10 为编译
(global-set-key (kbd "<f10>") 'compile)
;;自动跟随输出编译结果
(setq compilation-scroll-output t)
;; 跳过非error信息
(setq compilation-skip-threshold 2)
;; 高亮源代码中错误行
(setq next-error-highlight t)
;; 当前出错信息显示在buffer顶部距离
(setq compilation-context-lines 1)


;;--------------------dash--------------------

;; (add-to-list 'load-path "~/.emacs.d/elpa/dash-20151216.1315")
;; (require 'dash)

;;--------------------Debug--------------------

(setq gdb-many-windows t)

;;--------------------cc-mode--------------------

(setq-default c-basic-offset 4) ;;基本缩进量为4

(setq c-default-style '((java-mode . "java")
			(awk-mode . "awk")
			(other . "k&r")))

(setq-default c-electric-flag nil)
;Customizations for all modes in CC Mode.
;;代码折叠

(setq c-mode-hook
      '(lambda ()
	 (company-mode 1)
	 (linum-mode 1)
	 (abbrev-mode 1)
	 (hs-minor-mode 1)
	 (flycheck-mode 1)
	 (c-toggle-auto-hungry-state 1)
	 ))

;;--------------------配置packages源--------------------

(require 'package)
(setq package-archives '(("melpa" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/melpa/")
			 ("gnu" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/gnu/")
			 ("org" . "http://mirrors.tuna.tsinghua.edu.cn/elpa/org/")))
      
(package-initialize)

;;--------------------系统设置--------------------

(put 'set-goal-column 'disabled nil)
;; (custom-set-variables
;;  ;; custom-set-variables was added by Custom.
;;  ;; If you edit it by hand, you could mess it up, so be careful.
;;  ;; Your init file should contain only one such instance.
;;  ;; If there is more than one, they won't work right.
;;  '(custom-safe-themes
;;    (quote
;;     ("06f0b439b62164c6f8f84fdda32b62fb50b6d00e8b01c2208e55543a6337433a" default)))
 
;; ; '(window-numbering-mode t))
;; (custom-set-faces
;;  ;; custom-set-faces was added by Custom.
;;  ;; If you edit it by hand, you could mess it up, so be careful.
;;  ;; Your init file should contain only one such instance.
;;  ;; If there is more than one, they won't work right.
;;  )
;; (put 'narrow-to-page 'disabled nil)


(put 'narrow-to-region 'disabled nil)
(put 'scroll-left 'disabled nil)


(put 'upcase-region 'disabled nil)
(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(frame-background-mode (quote dark)))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )


;--------------------helm-mode--------------------

;; (add-to-list 'load-path "~/.emacs.d/elpa/helm-20160929.1313")
;; (add-to-list 'load-path "~/.emacs.d/elpa/helm-core-20160929.1313")

;; (require 'helm-config)
;; (require 'helm)
;; (helm-mode 1)
;; (semantic-mode 1);;开启语法解析模块

;; (setq helm-split-window-in-side-p           t ; open helm buffer inside current window, not occupy whole other window
;;       helm-move-to-line-cycle-in-source     t ; move to end or beginning of source when reaching top or bottom of source.
;;       helm-ff-search-library-in-sexp        t ; search for library in `require' and `declare-function' sexp.
;;       helm-scroll-amount                    8 ; scroll 8 lines other window using M-<next>/M-<prior>
;;       helm-ff-file-name-history-use-recentf t)

;;helm-session-buffer大小
;; (helm-autoresize-mode -1) ;自动调节session大小
;; (setq helm-autoresize-max-height 20) ;;最高不超过当前窗口高度的0%
;; (add-to-list 'helm-sources-using-default-as-input 'helm-source-man-pages);;将当前光标下的符号作为默认man-page的输入

;; ;;键绑定
;; (global-set-key (kbd "C-c h") 'helm-command-prefix);; helm-mode的命令前缀
;; (define-key helm-map (kbd "<tab>") 'helm-execute-persistent-action)
;; (global-unset-key (kbd "C-x c"))
;; (global-set-key (kbd "M-x") 'helm-M-x)
;; (global-set-key (kbd "M-y") 'helm-show-kill-ring)
;; (global-set-key (kbd "M-o") 'helm-mini)
;; (global-set-key (kbd "C-x C-f") 'helm-find-files)
;; (global-set-key (kbd "C-c h o") 'helm-occur)
;; (global-set-key (kbd "C-h SPC") 'helm-all-mark-rings)
;; (global-set-key (kbd "C-c h r") 'helm-register)
;; (global-set-key (kbd "C-c h f") 'helm-find)

;; (define-key minibuffer-local-map (kbd "C-c C-l") 'helm-minibuffer-history)

;; (setq helm-M-x-fuzzy-match t
;; helm-buffers-fuzzy-matching t
;; helm-recentf-fuzzy-match t
;; helm-semantic-fuzzy-match t
;; helm-imenu-fuzzy-match    t
;; ;helm-locate-fuzzy-match t
;; helm-apropos-fuzzy-match t
;; helm-lisp-fuzzy-completion t)

;--------------------helm-gtags--------------------
;(add-to-list 'load-path "~/.emacs.d/elpa/helm-gtags-20160")

;(require 'helm-gtags)
;; Enable helm-gtags-mode

;; (setq
;;  helm-gtags-ignore-case t
;;  helm-gtags-auto-update t
;;  helm-gtags-use-input-at-cursor t
;;  helm-gtags-pulse-at-cursor t
;;  helm-gtags-prefix-key "\C-cg"
;;  helm-gtags-suggested-key-mapping t)

;; (add-hook 'dired-mode-hook 'helm-gtags-mode)
;; (add-hook 'eshell-mode-hook 'helm-gtags-mode)
;; (add-hook 'c-mode-hook 'helm-gtags-mode)
;; (add-hook 'c++-mode-hook 'helm-gtags-mode)
;; (add-hook 'asm-mode-hook 'helm-gtags-mode)
 
;; (define-key helm-gtags-mode-map (kbd "C-c g a") 'helm-gtags-tags-in-this-function)
;; (define-key helm-gtags-mode-map (kbd "C-c g r") 'helm-gtags-find-rtag)
;; (define-key helm-gtags-mode-map (kbd "C-c g s") 'helm-gtags-find-symbol)
;; (define-key helm-gtags-mode-map (kbd "C-j") 'helm-gtags-select)
;; (define-key helm-gtags-mode-map (kbd "M-.") 'helm-gtags-dwim)
;; (define-key helm-gtags-mode-map (kbd "M-,") 'helm-gtags-pop-stack)
;; (define-key helm-gtags-mode-map (kbd "C-c <") 'helm-gtags-previous-history)
;; (define-key helm-gtags-mode-map (kbd "C-c >") 'helm-gtags-next-history)


;; ;;--------------------Tabbar Ruler--------------------
;; (setq tabbar-ruler-global-tabbar t)    ; get tabbar
;; (setq tabbar-ruler-global-ruler t)     ; get global ruler
;; (setq tabbar-ruler-popup-menu t)       ; get popup menu.
;; (setq tabbar-ruler-popup-toolbar t)    ; get popup toolbar
;; (setq tabbar-ruler-popup-scrollbar t)  ; show scroll-bar on mouse-move
;; (require 'tabbar-ruler)
;; (global-set-key (kbd "C-c t") 'tabbar-ruler-move)



;;--------------------chinese-pyim--------------------

;; (require 'chinese-pyim)
;; (require 'chinese-pyim-basedict)
;; (chinese-pyim-greatdict-enable)
;; (setq default-input-method "chinese-pyim")
;; (global-set-key (kbd "C-\\") 'toggle-input-method)
;; (setq pyim-use-tooltip 'popup)
;; (setq pyim-use-tooltip 'pos-tip)
;; (require 'chinese-pyim-company)
;; (setq pyim-company-max-length 6)
;; (setq pyim-page-length 10) ;每页显示10个候选项
;; (setq pyim-use-tooltip 'pos-tip)
;; (setq x-gtk-use-system-tooltips t)


;;----------------------TRAMP--------------------
;; 默认ssh连接
(setq tramp-default-method "ssh")



;; --------------------ivy-mode--------------------
(add-to-list 'load-path "~/.emacs.d/elpa/ivy-20161025.2304")

(require 'ivy)
(ivy-mode 1)

(setq ivy-use-virtual-buffers t) ;在切换buffer时, 是否显示不存在的buffer(如最近打开过的文件)
(setq ivy-height 10) ;每次显示10个候选项 
(setq ivy-count-format "(%d/%d) ") ;显示格式
(setq ivy-extra-directories nil) ;匹配时不显示./和../

(global-set-key (kbd "C-s") 'swiper)
(global-set-key (kbd "C-c C-r") 'ivy-resume)
(global-set-key (kbd "M-x") 'counsel-M-x)
(global-set-key (kbd "C-x C-f") 'counsel-find-file)

(global-set-key (kbd "<f1> f") 'counsel-describe-function)
(global-set-key (kbd "<f1> v") 'counsel-describe-variable)
(global-set-key (kbd "<f1> l") 'counsel-load-library)
(global-set-key (kbd "<f1> s") 'counsel-info-lookup-symbol)
;; (global-set-key (kbd "<f2> u") 'counsel-unicode-char)

(global-set-key (kbd "C-c g") 'counsel-git)
(global-set-key (kbd "C-c j") 'counsel-git-grep)
(global-set-key (kbd "C-c k") 'counsel-ag)
(global-set-key (kbd "C-x l") 'counsel-locate)

(global-set-key (kbd "C-c C-r") 'ivy-resume)
(define-key read-expression-map (kbd "C-r") 'counsel-expression-history)

;; 匹配时的颜色
(set-face-background 'ivy-current-match "purple3")
(set-face-foreground 'ivy-current-match nil)

(set-face-foreground 'ivy-minibuffer-match-face-1 "peach puff")
(set-face-background 'ivy-minibuffer-match-face-1 nil)

(set-face-foreground 'ivy-minibuffer-match-face-2 "peach puff")
(set-face-background 'ivy-minibuffer-match-face-2 nil)

(set-face-foreground 'ivy-minibuffer-match-face-3 "peach puff")
(set-face-background 'ivy-minibuffer-match-face-3 "nil")

(set-face-foreground 'ivy-minibuffer-match-face-4 "peach puff")
(set-face-background 'ivy-minibuffer-match-face-4 "nil")


;;--------------------org-mode--------------------

(add-hook 'org-mode-hook (lambda () (setq truncate-lines nil)))
(global-set-key "\C-cl" 'org-store-link)
(global-set-key "\C-ca" 'org-agenda)
(global-set-key "\C-cc" 'org-capture)
(global-set-key "\C-cb" 'org-iswitchb)

;; 记录TODO任务完成时(DNOE)的时间

(global-set-key (kbd "M-S-<down>") 'org-move-subtree-down)


;; --------------------Ebib--------------------
;; 启动ebib
(global-set-key "\C-ce" 'ebib)
;; 自动生成entry key
(setq ebib-autogenerate-keys nil)
;;为每个新加入的项生成其加入时间戳.
(setq ebib-use-timestamp t)
