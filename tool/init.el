;; .emacs

;; /cygdrive/c/Users/*/AppData/Roaming/.emacs

(provide '.emacs)

;; Constants
(setq path "~/.emacs.d/init.el")
(setq winh 46)

;; Frame
(defun reset () (interactive)
  (set-frame-position (selected-frame) 0 0)
  (set-frame-width (selected-frame) 80)
  (set-frame-height (selected-frame) winh))
(reset)

(defun clean-exit () (interactive)
  (delete-trailing-whitespace)
  (save-buffers-kill-terminal))
(global-set-key [?\C-\\] 'clean-exit)

;; Mode Line
(line-number-mode 1)
(column-number-mode 1)

;; Window
(set-background-color "#080808")
(set-cursor-color "#00ffff")
(setq inhibit-startup-screen 1)

(defun split-horizontally () (interactive)
  (set-frame-width (selected-frame) 164)
  (split-window-horizontally))
(global-set-key [?\C-\S-k ?\C-\S-l] 'split-horizontally)
(global-set-key [?\C-\S-k ?\C-\S-k] 'split-window-vertically)
(global-set-key [?\C-\S-j] 'previous-multiframe-window)
(global-set-key [?\C-\S-l] 'next-multiframe-window)
(global-set-key [?\C-\S-i] 'delete-window)

;; Buffer
(global-set-key [?\C-\S-a] 'previous-buffer)
(global-set-key [?\C-\S-d] 'next-buffer)

;; Text
(set-foreground-color "#ffffff")
(global-set-key [?\C-\-] 'text-scale-decrease)
(global-set-key [?\C-\=] 'text-scale-increase)

;; File
(defun init () (interactive) (find-file path))
(global-set-key [?\C-o] 'find-file)

(defun clean-save () (interactive)
  (delete-trailing-whitespace)
  (save-buffer)
  (if (string= (buffer-file-name) path)
      (load-file path) ()))
(global-set-key [?\C-s] 'clean-save)

(defun clean-write-file (new_file) (interactive "sWrite file: ")
  (delete-trailing-whitespace)
  (write-file new_file))
(global-set-key [?\C-\S-s] 'clean-write-file)

;; Navigate
(global-set-key [?\C-g] 'goto-line)

(global-set-key [?\C-f] 'isearch-forward)
(global-set-key [?\C-\S-f] 'isearch-forward-regexp)
(global-set-key [?\C-r] 'replace-string)
(global-set-key [?\C-\S-r] 'replace-regexp)

(global-set-key [?\M-j] 'backward-char)
(global-set-key [?\M-l] 'forward-char)
(global-set-key [?\M-i] 'previous-line)
(global-set-key [?\M-k] 'next-line)

(global-set-key [?\C-j] 'backward-word)
(global-set-key [?\C-l] 'forward-word)
(global-set-key [?\C-i] 'backward-paragraph)
(global-set-key [?\C-k] 'forward-paragraph)

(global-set-key [?\C-\M-j] 'beginning-of-line)
(global-set-key (kbd "C-<left>") 'beginning-of-line)
(global-set-key [?\C-\M-l] 'end-of-line)
(global-set-key (kbd "C-<right>") 'end-of-line)
(global-set-key [?\C-\M-i] 'beginning-of-buffer)
(global-set-key [?\C-h ?\C-i] 'beginning-of-buffer)
(global-set-key (kbd "C-<up>") 'beginning-of-buffer)
(global-set-key [?\C-\M-k] 'end-of-buffer)
(global-set-key (kbd "C-<down>") 'end-of-buffer)

;; Mark
(global-set-key [?\C-a] 'mark-whole-buffer)

(defun toggle-mark () (interactive)
  (if (equal (buffer-local-value 'mark-active (current-buffer)) t)
      (deactivate-mark) (set-mark-command nil)))
(global-set-key (kbd "C-SPC") 'toggle-mark)

;; Copy
(delete-selection-mode 1)
(global-set-key [?\C-x] 'kill-region)
(global-set-key [?\C-v] 'yank)
(global-set-key [?\C-c] 'kill-ring-save)
(global-set-key [?\C-z] 'undo)

;; Whitespace
(setq-default indent-tabs-mode nil)
(global-set-key [?\C-w] 'delete-horizontal-space)

(global-set-key [tab] 'indent-relative)
(define-key minibuffer-local-map (kbd "<tab>") 'minibuffer-complete)

(defun indent-left () (interactive)
  (indent-rigidly (region-beginning) (region-end) -1))
(defun indent-right () (interactive)
  (indent-rigidly (region-beginning) (region-end) 1))
(global-set-key (kbd "<C-tab>") 'indent-right)
(global-set-key [?\C-q] 'indent-left)

(setq-default electric-indent-mode nil)
(defun newline-then-indent () (interactive) (newline) (indent-relative-maybe))
(global-set-key (kbd "RET") 'newline-then-indent)

;; Comment
(global-set-key [?\C-h ?\C-c] 'comment-region)
(global-set-key [?\C-h ?\C-u] 'uncomment-region)

;; Block
(add-hook 'c-mode-common-hook
  (lambda()
    (global-set-key [?\M-u] 'hs-hide-block)
    (global-set-key [?\M-o] 'hs-show-block)
    (global-set-key [?\C-\M-u] 'hs-hide-all)
    (global-set-key [?\C-\M-o] 'hs-show-all)
    (hs-minor-mode t)))

;; Tools
(setq explicit-shell-file-name "c:/cygwin64/bin/bash")
(global-set-key [?\C-p] 'calc)

;; Backup
;; (setq backup-directory-alist
;;      '(("." . (concat (getenv "HOME") "\\.emacs.d\\backup"))))

;; Autocomplete
;; (require 'yasnippet)
;; (yas-global-mode 1)
;; (require 'auto-complete-config)
;; (add-to-list 'ac-dictionary-directories "~/.emacs.d/ac-dict")
;; (ac-config-default)
;; (ac-set-trigger-key "TAB")
;; (ac-set-trigger-key "<tab>")
